/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include "ID.hh"
#include <fstream>
#include <sstream>
#include <iterator>
#include "Logger.hh"
#include "Assert.hh"
#include "Resource.hh"
#include <XML/rapidxml_utils.hpp>
using namespace Logger;

namespace IOManager {
	// Base test function for XML purposes (TODO)
	static void LevelParameters(std::string &&filename, Difficulty dif, int &hpLeft, float &velocity, float &velocityMod) {
		rapidxml::file<> xmlFile(RESOURCE_FILE(filename));
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());
		rapidxml::xml_node<> *root_node = doc.first_node("level");
		hpLeft = atoi(root_node->first_node("constants")->first_attribute("kValue")->value());
		velocity = (float)(atof(root_node->first_node("constants")->first_attribute("zValue")->value()));
		for (rapidxml::xml_node<> * node = root_node->first_node("difficulty"); node; node = node->next_sibling()) {
			if (atoi(node->first_attribute("difficultyID")->value()) == (int)dif) {
				rapidxml::xml_node<> * n = node->first_node("difficultyName");
				Println("xml data for ", n->value(), " difficulty");
				Println("kValue is: ", hpLeft, " and zValue: ", velocity);

				n = node->first_node("param");
				hpLeft *= atoi(n->first_attribute("livesFactor")->value());
				velocity *= atoi(n->first_attribute("velocity0Factor")->value());
				velocityMod = (float)(atof(n->first_attribute("velocityMod")->value()));

				Println("livesFactor: ", n->first_attribute("livesFactor")->value(), " player will start with a total of: ", hpLeft, "lives");
				Println("initialVelocity: ", n->first_attribute("velocity0Factor")->value(), " velocity is set to: ", velocity, "units");
				Println("velocityModifier will be: ", n->first_attribute("velocityMod")->value());
			}
		}
	}

	// Loader function that takes level info for a grid
	static std::vector<std::vector<ObjectID>> LoadLevel(std::string &&filename, int &rows, int &cols) {
		std::ifstream fileData(RESOURCE_FILE(filename));
		ASSERT(fileData.good());
		std::vector<std::vector<ObjectID>> lvlData;
		std::string line;
		while (std::getline(fileData, line)) {
			std::istringstream strData(std::move(line));
			lvlData.emplace_back(std::istream_iterator<ObjectID>(std::move(strData)), std::istream_iterator<ObjectID>());
		}
		rows = int(lvlData.size()); cols = int(lvlData[0].size());
		fileData.close();
		#pragma region DEBUG_DATA
			Println("Level: ", filename);
			for (auto &r : lvlData) {
				for (auto &c : r) Print(c, ' ');
				Endln();
			}
		#pragma endregion
		return std::move(lvlData);
	}

}