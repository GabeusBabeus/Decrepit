#include "HorizontalScroll.h"
#include <iostream>
using namespace std;
HorizontalScroll::HorizontalScroll()
{
}

void HorizontalScroll::Update()
{
	//Above focus
	if (m_focus->GetPosition().x > m_cam->m_localPosition.x + m_offset)
	{
		//Calculate the amount the focus has "pushed" the camera right by
		float difference = m_focus->GetPosition().x - (m_cam->m_localPosition.x + m_offset);
		//cout << m_cam->m_localPosition.x << endl;
		//Adjust the camera
		if (m_cam->m_localPosition.x > 376) { //STOP THE CAM FROM GOING TO FAR RIGHT
			difference = 0.f;
		}
		m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z));
	}

	//Below focus
	if (m_focus->GetPosition().x < m_cam->m_localPosition.x - m_offset)
	{
		//Calculate the amount the focus has "pushed" the camera left by
		float difference = m_focus->GetPosition().x - (m_cam->m_localPosition.x - m_offset);
		

		if (spawnCam) {
			m_cam->SetPosition(vec3(-37, m_cam->GetPosition().y, m_cam->GetPosition().z));
			setSpawnCam(false);
		}
		//Adjust the camera
		if (m_cam->m_localPosition.x < -36) { //STOP THE CAM FROM GOING TO FAR LEFT
			difference = 0.f;
		}
		m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z));
		
		
	}
}

Camera * HorizontalScroll::GetCam() const
{
	return m_cam;
}

Transform * HorizontalScroll::GetFocus() const
{
	return m_focus;
}

float HorizontalScroll::GetOffset() const
{
	return m_offset;
}

void HorizontalScroll::SetCam(Camera * cam)
{
	m_cam = cam;
}

void HorizontalScroll::SetFocus(Transform * focus)
{
	m_focus = focus;
}

void HorizontalScroll::SetOffset(float offset)
{
	m_offset = offset;
}

void HorizontalScroll::setSpawnCam(bool torf)
{
	spawnCam = torf;
}
