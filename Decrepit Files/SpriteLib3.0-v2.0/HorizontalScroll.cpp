#include "HorizontalScroll.h"
#include <iostream>
#include "ECS.h"
using namespace std;
HorizontalScroll::HorizontalScroll()
{
}

void HorizontalScroll::Update()
{
	auto& level = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	//cout << level.enterLeft << endl;
	if (level.currLevel == "levelone") {
		if (level.enterRight) {
			m_cam->SetPosition(vec3(336.f, m_cam->GetPosition().y, m_cam->GetPosition().z));
			level.enterRight = false;
		}
		if (level.enterLeft) {
			m_cam->SetPosition(vec3(-21.f, m_cam->GetPosition().y, m_cam->GetPosition().z));
			level.enterLeft = false;
		}
	}
	if (level.currLevel == "toplevel") {
		if (level.enterLeft == true) {
			m_cam->SetPosition(vec3(956.f, m_cam->GetPosition().y, m_cam->GetPosition().z));
			level.enterLeft = false;
		}
	}
	if (level.currLevel == "leveltwo") {
		if (level.enterRight == true) {
			m_cam->SetPosition(vec3(2715.f, m_cam->GetPosition().y, m_cam->GetPosition().z));
			level.enterRight = false;
		}
	}
	if (level.currLevel == "basement") {
		if (level.enterLeft == true) {
			m_cam->SetPosition(vec3(1465.f, m_cam->GetPosition().y, m_cam->GetPosition().z));
			level.enterLeft = false;
		}
	}
	
	//Above focus
	if (m_focus->GetPosition().x > m_cam->m_localPosition.x + m_offset)
	{
		
		//cout << m_cam->m_localPosition.x << endl;
		if (level.currLevel == "levelone") {

			//Calculate the amount the focus has "pushed" the camera right by
			float difference = m_focus->GetPosition().x - (m_cam->m_localPosition.x + m_offset);
			//make adjustments
			if (m_cam->m_localPosition.x > 335) {
				difference = 0.f;
			}
			

			m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z));
		}
		if (level.currLevel == "endcard") {

			//Calculate the amount the focus has "pushed" the camera right by
			float difference = m_focus->GetPosition().x - (m_cam->m_localPosition.x + m_offset);
			//make adjustments
			


			m_cam->SetPosition(vec3(-3000, 0, m_cam->GetPosition().z));
		}
		if (level.currLevel == "basement") {
			cout << m_cam->m_localPosition.x << endl;

			//Calculate the amount the focus has "pushed" the camera right by
			float difference = m_focus->GetPosition().x - (m_cam->m_localPosition.x + m_offset);
			//make adjustments
			if (m_cam->m_localPosition.x > 1725) {
				difference = 0.f;
			}


			m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z));
		}
		if (level.currLevel == "leveltwo") {
			//cout << m_cam->m_localPosition.x << endl;
			//Calculate the amount the focus has "pushed" the camera right by
			float difference = m_focus->GetPosition().x - (m_cam->m_localPosition.x + m_offset);
			//make adjustments
			if (m_cam->m_localPosition.x > 2714) {
				difference = 0.f;
			}


			m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z));
		}
		if (level.currLevel == "toplevel") {
			cout << m_cam->m_localPosition.x << endl;

			//Calculate the amount the focus has "pushed" the camera right by
			float difference = m_focus->GetPosition().x - (m_cam->m_localPosition.x + m_offset);
			//make adjustments
			if (m_cam->m_localPosition.x > 1550) {
				difference = 0.f;
			}


			m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z));
		}
	
			

		if (level.inForest) {

			//Calculate the amount the focus has "pushed" the camera right by
			float difference = m_focus->GetPosition().x - (m_cam->m_localPosition.x + m_offset);
			//make adjustments


			m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z));
		}
	}
	
	//Below focus
	if (m_focus->GetPosition().x < m_cam->m_localPosition.x - m_offset)
	{
		
		if (level.currLevel == "levelone") {
			//cout << m_cam->m_localPosition.x << endl;
			
			//Calculate the amount the focus has "pushed" the camera left by
			float difference = m_focus->GetPosition().x - (m_cam->m_localPosition.x - m_offset);

			//make adjustments
			if (m_cam->m_localPosition.x < -20) {
				difference = 0.f;
			}

			m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z));
		}
		if (level.currLevel == "basement") {
			//cout << m_cam->m_localPosition.x << endl;

			//Calculate the amount the focus has "pushed" the camera left by
			float difference = m_focus->GetPosition().x - (m_cam->m_localPosition.x - m_offset);

			//make adjustments
			if (m_cam->m_localPosition.x < 1460) {
				difference = 0.f;
			}



			m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z));
		}
		if (level.currLevel == "leveltwo") {
			cout << m_cam->m_localPosition.x << endl;

			//Calculate the amount the focus has "pushed" the camera left by
			float difference = m_focus->GetPosition().x - (m_cam->m_localPosition.x - m_offset);

			//make adjustments
			if (m_cam->m_localPosition.x < 2480) {
				difference = 0.f;
			}



			m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z));
		}
		if (level.currLevel == "toplevel") {
			cout << m_cam->m_localPosition.x << endl;

			//Calculate the amount the focus has "pushed" the camera left by
			float difference = m_focus->GetPosition().x - (m_cam->m_localPosition.x - m_offset);

			//make adjustments
			if (m_cam->m_localPosition.x < 957) {
				difference = 0.f;
			}


			m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z));
		}
		if (level.inForest) {

			//Calculate the amount the focus has "pushed" the camera right by
			float difference = m_focus->GetPosition().x - (m_cam->m_localPosition.x - m_offset);
			//make adjustments


			m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z));
		}
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

void HorizontalScroll::setenterLeft(bool torf)
{
	enterLeft = torf;
}
