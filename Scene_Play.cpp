#include "Scene_Play.h"
#include "Physics.h"
#include "Assets.h"
#include "Game.h"
#include "Components.h"
#include "Scene_Menu.h"
#include "Action.hpp"

#include <iostream>
#include <fstream>

Scene_Play::Scene_Play(Game * game,const std::string& levelPath)
	: Scene(game)
	, m_levelPath(levelPath)
{
	init(m_levelPath);
}

void Scene_Play::init(const std::string& levelPath)
{
	registerAction(sf::Keyboard::P, "PAUSE");
	registerAction(sf::Keyboard::Escape, "QUIT");
	registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
	registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
	registerAction(sf::Keyboard::G, "TOGGLE_GRID");

	registerAction(sf::Keyboard::Up, "JUMP");
	registerAction(sf::Keyboard::Left, "MOVE_LEFT");
	registerAction(sf::Keyboard::Right, "MOVE_RIGHT");
	registerAction(sf::Keyboard::Down, "MOVE_DOWN");
	registerAction(sf::Keyboard::Space, "SHOOT");

	m_gridText.setCharacterSize(12);
	m_gridText.setFont(m_game->getAssets().getFont("Arial"));

	loadLevel(levelPath);
}

Vec2f Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	return Vec2f(0, 0);
}

void Scene_Play::loadLevel(const std::string& filename)
{

	m_entityManager = EntityManager();

	std::ifstream f(filename);
	std::string str;
	std::cout << filename << std::endl;
	if (!f.good()) {
		std::cerr << "Failed to open level file: " << filename << std::endl;
		return;
	}
	while (f.good()) { 
		f >> str;
		if (str == "Tile") {
			std::string type; size_t x, y;
			f >> type >> x >> y;
			auto e = m_entityManager.addEntity("tile");
			e->addComponent<CTransform>(Vec2f(x, y));
			bool repeat = true;
			if (type == "Explosion" || type == "Coin") repeat = false;
			e->addComponent<CAnimation>(m_game->getAssets().getAnimation(type), repeat);
			e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
			// add a bounding box, this will now show up if we press the 'C' Key
			e->addComponent<CBoundingBox>(m_game->getAssets().getAnimation(type).getSize());
		}
		else if (str == "Dec") {
			std::string type; float x, y;
			f >> type >> x >> y;
			auto e = m_entityManager.addEntity("dec");
			e->addComponent<CTransform>(Vec2f(x, y));
			e->addComponent<CAnimation>(m_game->getAssets().getAnimation(type), true);
		}
		else {
			f >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CX >> m_playerConfig.CY
				>> m_playerConfig.SPEED >> m_playerConfig.JUMP >> m_playerConfig.MAXSPEED >> m_playerConfig.GRAVITY
				>> m_playerConfig.WEAPON;
			spawnPlayer();
		}
	}
}

void Scene_Play::spawnPlayer()
{
	// check to see i plyer already exists
	if (!m_player) { m_player = m_entityManager.addEntity("player");}
	m_player = m_entityManager.addEntity("player");
	m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("Stand"), true);
	m_player->addComponent<CTransform>(Vec2f(m_playerConfig.X, m_playerConfig.Y));
	m_player->getComponent<CTransform>().prevPos = Vec2f(m_playerConfig.X, m_playerConfig.Y);
	m_player->addComponent<CBoundingBox>(Vec2f(m_playerConfig.CX, m_playerConfig.CY));
	m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
	m_player->addComponent<CInput>().canJump = false;

	//playMusic();

	// helps with state management(ground or air)
	auto e = m_entityManager.addEntity("box");
	e->addComponent<CTransform>(Vec2f(m_playerConfig.X, m_playerConfig.Y - 1));
	//e->getComponent<CTransform>().velocity = Vec2f(eConfig.SPEED, eConfig.JUMP);
	e->addComponent<CBoundingBox>(Vec2f(m_playerConfig.CX, m_playerConfig.CY));
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
	auto bullet = m_entityManager.addEntity("bullet");
	bullet->addComponent<CAnimation>(m_game->getAssets().getAnimation(m_playerConfig.WEAPON), true);
	bullet->addComponent<CTransform>(entity->getComponent<CTransform>().pos);
	bullet->addComponent<CBoundingBox>(m_game->getAssets().getAnimation(m_playerConfig.WEAPON).getSize());
	bullet->getComponent<CTransform>().velocity = { (m_player->getComponent<CTransform>().scale.x * (m_playerConfig.SPEED * 2)), 0.0f };
	bullet->addComponent<CLifespan>(90, m_currentFrame);
}

void Scene_Play::update()
{
	if (!m_paused) {
		m_entityManager.update();
		sMovement();
		sCollision();
		sLifespan();
		m_currentFrame++;
		sAnimation();
	}
	sRender();
}

void Scene_Play::sMovement()
{
	Vec2f& ppos = m_player->getComponent<CTransform>().pos;
	std::shared_ptr<Entity> box = m_entityManager.getEntities("box")[0];
	box->getComponent<CTransform>().pos = Vec2f(ppos.x, ppos.y - 1);


	Vec2f playerVelocity = { 0.0f, m_player->getComponent<CTransform>().velocity.y };

	// falling
	//if (m_player->getComponent<CState>().state == "AIR") {
	//	m_player->getComponent<CInput>().canJump = false;
	//	ppos.y += playerVelocity.y;
	//}

	// jump
	if (m_player->getComponent<CInput>().up && m_player->getComponent<CInput>().canJump) {
		playerVelocity.y = m_playerConfig.JUMP;
		m_player->getComponent<CInput>().canJump = false;
		m_player->getComponent<CState>().state = "AIR";
	}
	if (!m_player->getComponent<CInput>().up && m_player->getComponent<CTransform>().velocity.y > 0) {
		playerVelocity.y = 0;
	}
	//move left right whether on ground or air
	if (m_player->getComponent<CInput>().left) {
		playerVelocity.x = -m_playerConfig.SPEED;
		m_player->getComponent<CTransform>().scale.x = -1.0;
	}
	if (m_player->getComponent<CInput>().right) {
		playerVelocity.x = m_playerConfig.SPEED;
		m_player->getComponent<CTransform>().scale.x = 1.0;
	}


	// on ground
	playerVelocity.y += m_player->getComponent<CGravity>().gravity;
	if (m_player->getComponent<CState>().state == "GROUND") {
		m_player->getComponent<CInput>().canJump = false;
		playerVelocity.y = 0;
	}

	if (abs(playerVelocity.y) > m_playerConfig.MAXSPEED) {
		playerVelocity.y = m_playerConfig.MAXSPEED * ((playerVelocity.y) / abs(playerVelocity.y));
	}
	if (abs(playerVelocity.x) > m_playerConfig.MAXSPEED) {
		playerVelocity.x = m_playerConfig.MAXSPEED * ((playerVelocity.x) / abs(playerVelocity.x));
	}


	m_player->getComponent<CTransform>().prevPos = m_player->getComponent<CTransform>().pos; // save previous position

	m_player->getComponent<CTransform>().velocity = playerVelocity;
	m_player->getComponent<CTransform>().pos.x += playerVelocity.x;
	m_player->getComponent<CTransform>().pos.y += playerVelocity.y;


	for (auto e : m_entityManager.getEntities("bullet")) {
		e->getComponent<CTransform>().pos.x += e->getComponent<CTransform>().velocity.x;
	}
}

void Scene_Play::sLifespan()
{
	for (auto e : m_entityManager.getEntities()) {
		if (e->hasComponent<CLifespan>()) {
			if (e->getComponent<CLifespan>().frameCreated + e->getComponent<CLifespan>().lifespan <= m_currentFrame) {
				e->destroy();
			}
		}
	}
}

void Scene_Play::explosion(std::shared_ptr<Entity> e) {
	auto expo = m_entityManager.addEntity("explosion");
	expo->addComponent<CAnimation>(m_game->getAssets().getAnimation("Explosion"), false);
	expo->addComponent<CTransform>(e->getComponent<CTransform>().pos);

}

void Scene_Play::spawnCoin(std::shared_ptr<Entity> e) {
	auto coin = m_entityManager.addEntity("coin");
	coin->addComponent<CTransform>(Vec2f(e->getComponent<CTransform>().pos.x, e->getComponent<CTransform>().pos.y + 64));
	coin->addComponent<CAnimation>(m_game->getAssets().getAnimation("Coin"), false);
}

void Scene_Play::sCollision()
{
	for (auto b : m_entityManager.getEntities("bullet")) {
		for (auto t : m_entityManager.getEntities("tile")) {
			auto overlap = Physics::GetOverlap(b, t);
			if (overlap.x > 0 && overlap.y > 0) {
				b->destroy();
				if (t->getComponent<CAnimation>().animation.getName() == "Brick") {
					explosion(t);
					t->destroy();
				}
			}
		}
	}


	// Implement player / tile collisions and resolutions
	// Update the CState component of the player to store whether
	// it is currently on the ground or in the air. This will be
	// used by the Animation system
	for (auto t : m_entityManager.getEntities("tile")) {
		auto overlap = Physics::GetOverlap(m_player, t);
		auto ppos = m_player->getComponent<CTransform>().pos;
		auto tpos = t->getComponent<CTransform>().pos;
		auto prevOverlap = Physics::GetPreviousOverlap(m_player, t);
		if (overlap.x > 0 && overlap.y > 0) {
			if (prevOverlap.y > 0) { // for collisions on the sides
				if (m_player->getComponent<CTransform>().pos.x > t->getComponent<CTransform>().pos.x) {
					m_player->getComponent<CTransform>().pos.x += overlap.x;
				}
				else {
					m_player->getComponent<CTransform>().pos.x -= overlap.x;
				}
				auto animation = t->getComponent<CAnimation>().animation.getName();
				if (animation == "Flag" || animation == "Pole" || animation == "PoleTop") {
					m_player->destroy();
					spawnPlayer();
				}
			}
			if (prevOverlap.x > 0) { // for vertical collisions
				if (m_player->getComponent<CTransform>().pos.y > t->getComponent<CTransform>().pos.y) { // falling down
					m_player->getComponent<CTransform>().pos.y += overlap.y;
					m_player->getComponent<CState>().state = "GROUND"; // todo : when to switch to air
				}
				else { // going  up
					m_player->getComponent<CTransform>().pos.y -= overlap.y;
					m_player->getComponent<CTransform>().velocity.y = 0;
					if (t->getComponent<CAnimation>().animation.getName() == "Brick") {
						explosion(t);
						t->destroy();
					}
					else if (t->getComponent<CAnimation>().animation.getName() == "Question") {
						spawnCoin(t);
						t->getComponent<CAnimation>().animation = m_game->getAssets().getAnimation("Question2");
					}
				}
			}
		}
	}



	// this is a hacky solution to manage player state (ground or air) but i can only think of this for now
	// maybe later move the line that changes position to sMovement
	m_player->getComponent<CState>().state = "AIR";
	for (auto t : m_entityManager.getEntities("tile")) {

		if (m_player) {
			auto ppos = m_player->getComponent<CTransform>().pos;
			auto box = m_entityManager.getEntities("box")[0];
			auto overlap = Physics::GetOverlap(box, t);
			if (overlap.x > 0 && overlap.y > 0) {
				m_player->getComponent<CState>().state = "GROUND";
				m_player->getComponent<CInput>().canJump = true;
			}
		}
	}
}

void Scene_Play::sDoAction(const Action& action)
{
	if (action.getType() == "START")
	{
		if (action.getName() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
		else if (action.getName() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
		else if (action.getName() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
		else if (action.getName() == "PAUSE") { setPaused(!m_paused); }
		else if (action.getName() == "QUIT") { onEnd(); }
		else if (action.getName() == "JUMP") {
			m_player->getComponent<CInput>().up = true;
		}
		else if (action.getName() == "MOVE_LEFT") { m_player->getComponent<CInput>().left = true; }
		else if (action.getName() == "MOVE_RIGHT") { m_player->getComponent<CInput>().right = true; }
		else if (action.getName() == "SHOOT" && m_player->getComponent<CInput>().canShoot) {
			m_player->getComponent<CInput>().shoot = true;
			m_player->getComponent<CInput>().canShoot = false;
			spawnBullet(m_player);
		}
	}
	else if (action.getType() == "END") {
		if (action.getName() == "JUMP") {
			m_player->getComponent<CInput>().up = false;
		}
		else if (action.getName() == "MOVE_LEFT") { m_player->getComponent<CInput>().left = false; }
		else if (action.getName() == "MOVE_RIGHT") { m_player->getComponent<CInput>().right = false; }
		else if (action.getName() == "SHOOT") {
			m_player->getComponent<CInput>().shoot = false;
			m_player->getComponent<CInput>().canShoot = true;
		}
	}
}

void Scene_Play::sAnimation()
{
	auto state = m_player->getComponent<CState>().state;
	auto animation = m_player->getComponent<CAnimation>().animation.getName();
	if (state == "AIR") {
		m_player->getComponent<CAnimation>().animation = m_game->getAssets().getAnimation("Air");
	}
	else if (state == "GROUND" && m_player->getComponent<CTransform>().velocity.x == 0) {
		m_player->getComponent<CAnimation>().animation = m_game->getAssets().getAnimation("Stand");
		// only set the animation if it should change as it overwrites the previous animation 
		// (and currentAnimationFrame values start from 0 again)
		// it does not matter for others(the two above) cause they have 1 frame only
	}
	else if (state == "GROUND" && m_player->getComponent<CTransform>().velocity.x != 0) {
		m_player->getComponent<CAnimation>().animation = m_game->getAssets().getAnimation("Run");
		std::cout << "here";
	}


	// for each entity with an animation, call entity->getComponent<CAnimation>().animation.update()
	// if the animation is not repeated, and it has ended, destroy the entity
	for (auto& e : m_entityManager.getEntities()) {
		if (e->hasComponent<CAnimation>()) {
			e->getComponent<CAnimation>().animation.update();
			if (!e->getComponent<CAnimation>().repeat && e->getComponent<CAnimation>().animation.hasEnded()) {
				e->destroy();
			}
		}
	}
}

void Scene_Play::onEnd()
{
	m_hasEnded = true;
	m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game), false);
	//m_music.stop();
}

void Scene_Play::sGUI()
{
	// TODO
}

void Scene_Play::sRender()
{
	// color the background darker so you know that the game is paused
	if (!m_paused) { m_game->getWindow().clear(sf::Color(100, 100, 255)); }
	else { m_game->getWindow().clear(sf::Color(50, 50, 150)); }

	// set the viewport of the window to be centered on the player if it's far enough right
	auto& pPos = m_player->getComponent<CTransform>().pos;
	float windowCenterX = std::max(m_game->getWindow().getSize().x / 2.0f, pPos.x);
	sf::View view = m_game->getWindow().getView();
	view.setCenter(windowCenterX, m_game->getWindow().getSize().y - view.getCenter().y);
	m_game->getWindow().setView(view);

	// draw all Entity textures / animations
	if (m_drawTextures)
	{
		for (auto& e : m_entityManager.getEntities())
		{
			auto& transform = e->getComponent<CTransform>();
			if (e->hasComponent<CAnimation>())
			{
				auto& animation = e->getComponent<CAnimation>().animation;
				animation.getSprite().setPosition(transform.pos.x, m_game->getWindow().getSize().y - transform.pos.y);
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setScale(transform.scale.x, transform.scale.y);
				m_game->getWindow().draw(animation.getSprite());
			}
		}
	}

	// draw all entity collision bounding boxes with a rectangle shape
	if (m_drawCollision)
	{
		for (auto& e : m_entityManager.getEntities())
		{
			if (e->hasComponent<CBoundingBox>())
			{
				if (e->tag() == "box") continue;
				auto& box = e->getComponent<CBoundingBox>();
				auto& transform = e->getComponent<CTransform>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
				rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
				rect.setPosition(transform.pos.x, m_game->getWindow().getSize().y - transform.pos.y);
				rect.setFillColor(sf::Color(0, 0, 0, 0));
				rect.setOutlineColor(sf::Color(255, 255, 255, 255));
				rect.setOutlineThickness(1);
				m_game->getWindow().draw(rect);
			}
		}
	}

	// draw the grid so that it is can easy to debug
	if (m_drawGrid)
	{
		float leftX = m_game->getWindow().getView().getCenter().x - width() / 2;
		float rightX = leftX + width() + m_gridSize.x;
		float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

		for (float x = nextGridX; x < rightX; x += m_gridSize.x)
		{
			drawLine(Vec2f(x, 0), Vec2f(x, (float)height()));
		}

		for (float y = 0; y < height(); y += m_gridSize.y)
		{
			drawLine(Vec2f(leftX, height() - y), Vec2f(rightX, height() - y));

			for (float x = nextGridX; x < rightX; x += m_gridSize.x)
			{
				std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
				std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
				m_gridText.setString("(" + xCell + "," + yCell + ")");
				m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
				m_game->getWindow().draw(m_gridText);
			}
		}
	}
}

void Scene_Play::drawLine(const Vec2f& p1, const Vec2f& p2)
{
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(p1.x, p1.y)),
		sf::Vertex(sf::Vector2f(p2.x, p2.y))
	};
	m_game->getWindow().draw(line, 2, sf::Lines);
}