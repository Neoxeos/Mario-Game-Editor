#include "Animation.h"


void Animation::update()
{
	m_currentFrame++;
	if (m_speed == 0) return; // if speed is 0, we never switch frames
	m_currentFrame = (m_currentFrame + 1) % m_framecount;
}

bool Animation::hasEnded() const
{
	return m_currentFrame == m_framecount - 1;
}