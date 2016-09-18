#pragma once

#include "hitable.h"
#include <vector>

class HitableList : public Hitable
{
public:
	HitableList(){}
	HitableList(std::vector<Hitable*>*list) { this->list = list; }

	// ヒットしたリスト中のHitableの点、で最もカメラに近い点を記録する
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const;

	size_t getListSize()const { return list->size(); }
	Hitable* operator[](int i) { return (*list)[i]; }

	// TODO: implement destructor to release vector's resources.
private:
	std::vector<Hitable*>* list;
};