#include "Scroll.h"

int Scroll::scrollOneEntity = 0;
int Scroll::scrollTwoEntity = 0;
int Scroll::scrollThreeEntity = 0;
int Scroll::scrollFourEntity = 0;
int Scroll::scrollFiveEntity = 0;
int Scroll::scrollSixEntity = 0;




void Scroll::setScrollEntity(int num, int entity) {
	if (num == 1) {
		scrollOneEntity = entity;
	}
	if(num == 2) {
		scrollTwoEntity = entity;
	}
	if(num == 3) {
		scrollThreeEntity = entity;
	}
	if(num == 4) {
		scrollFourEntity = entity;
	}
	if(num == 5) {
		scrollFiveEntity = entity;
	}
	if (num == 6) {
		scrollSixEntity = entity;
	}
}
int Scroll::getScrollEntity(int num) {
	if (num == 1) {
		return scrollOneEntity;
	}
	if (num == 2) {
		return scrollTwoEntity;
	}
	if (num == 3) {
		return scrollThreeEntity;
	}
	if (num == 4) {
		return scrollFourEntity;
	}
	if (num == 5) {
		return scrollFiveEntity;
	}
	if (num == 6) {
		return scrollSixEntity;
	}
}