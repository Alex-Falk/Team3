#include "DataDrivenMap.h"
#include "WeaponPickup.h"
#include "PaintPool.h"
#include "Game.h"
#include "MultiPaintPool.h"
#include <nclgl\Lexer.h>
#include <ncltech\CommonUtils.h>


//--------------------------------------------------------------------------------------------//
// Initialisation and Cleanup
//--------------------------------------------------------------------------------------------//
void DataDrivenMap::OnInitializeScene()
{
	OnInitializeGUI();

	ReadFile();

	Buildmap();

	for (uint i = 1; i < 5; i++) {
		BuildObject(GetObjects(lines[i]));
	}

	Map::OnInitializeScene();
}


void DataDrivenMap::AddObjects() {
	BuildObjects();

}

void DataDrivenMap::OnUpdateScene(float dt)
{
	perfMapObjects.UpdateRealElapsedTime(dt);
	Map::OnUpdateScene(dt);

	perfMapObjects.BeginTimingSection();

}

//--------------------------------------------------------------------------------------------//
// Utility. TODO: can probably (re)move this
//--------------------------------------------------------------------------------------------//
void DataDrivenMap::onButtonClicked()
{
	SceneManager::Instance()->JumpToScene();
}

void DataDrivenMap::ReadFile() {
	std::ifstream ifs(fileName);
	std::string file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	lines = GetLines(file);
	cout << "file ok";
	ifs.close();
}

void DataDrivenMap::Buildmap() {
	BuildObject(GetObjects(lines[0]));
}

void DataDrivenMap::BuildObjects() {

	int i = 5;
	while (i < lines.size())
	{
		vector<string>object = GetObjects(lines[i]);
		BuildObject(object);
		i++;
	}
}

void DataDrivenMap::BuildObject(vector<std::string> object) {

	if (object[0] == "CAPTURE_AREA")		AddCaptureAreas(object);
	else if (object[0] == "CUBE")			AddCuboid(object);
	else if (object[0] == "PICKUP")			AddPickups(object);
	else if (object[0] == "WEAPON_PICKUP")	AddWeaponPickups(object);
	else if (object[0] == "PAINT_POOL")		AddPaintPools(object);
	else if (object[0] == "MULTI_POOL")		AddMultiPaintPools(object);
	else if (object[0] == "MINION_AREA")	AddMinionAreas(object);
	else if (object[0] == "GROUND")			AddGround(object);
	else if (object[0] == "SPAWN_LOC")		SetSpawnLocation(object);
}

vector<string> DataDrivenMap::GetLines(std::string file) {
	vector<string> line;
	line = split(file, '\n');
	return line;
}

vector<string> DataDrivenMap::GetObjects(std::string line) {
	vector<string> object;
	object = split(line, ' ');
	//if (object[0] == "\n" && object[0] == "//")
	//	numemptyline++;
	//else
	return object;
}

void DataDrivenMap::AddGround(vector<std::string> object) {
	Map::SetMapDimensions(Vector2(stof(object[1]), stof(object[2])));
	BuildGround(Vector2(stof(object[1]), stof(object[2])));
}

void DataDrivenMap::SetSpawnLocation(vector<std::string> object) {
	sPotision[stoi(object[1])] = Vector3(stof(object[2]), stof(object[3]), stof(object[4]));
}

void DataDrivenMap::SetSpawnLocations() {
	GUIsystem::Instance()->SetDrawScoreBar(true);
	spawnPositions[0] = sPotision[0];
	spawnPositions[1] = sPotision[1];
	spawnPositions[2] = sPotision[2];
	spawnPositions[3] = sPotision[3];
}

void DataDrivenMap::AddPaintPools(vector<std::string> object) {

	if (object[4] == "RED")		team = RED;
	if (object[4] == "GREEN")	team = GREEN;
	if (object[4] == "BLUE")	team = BLUE;
	if (object[4] == "PINK")	team = PINK;
	AddPickup(new PaintPool(Vector3(stof(object[1]), stof(object[2]), stof(object[3])), team, object[5]));
}

void DataDrivenMap::AddPickups(vector<std::string> object) {

	if (object[4] == "SPEED_BOOST")		type = SPEED_BOOST;
	if (object[4] == "JUMP_BOOST")		type = JUMP_BOOST;
	AddPickup(new Pickup(Vector3(stof(object[1]), stof(object[2]), stof(object[3])), type, object[5]));
}

void DataDrivenMap::AddWeaponPickups(vector<std::string> object) {

	if (object[4] == "PAINT_SPRAY")			weapType = PAINT_SPRAY;
	if (object[4] == "PAINT_PISTOL")		weapType = PAINT_PISTOL;
	if (object[4] == "AUTO_PAINT_LAUNCHER")	weapType = AUTO_PAINT_LAUNCHER;
	if (object[4] == "PAINT_ROCKET")		weapType = PAINT_ROCKET;
	AddPickup(new WeaponPickup(Vector3(stof(object[1]), stof(object[2]), stof(object[3])), weapType, object[5], stof(object[6])));
}

void DataDrivenMap::AddCaptureAreas(vector<std::string> object) {
	AddCaptureArea(new CaptureArea(Vector3(stof(object[1]), stof(object[2]), stof(object[3])), object[4], Vector3(stof(object[5]), stof(object[6]), stof(object[7])), stof(object[8])));
}

void DataDrivenMap::AddMultiPaintPools(vector<std::string> object) {

	Pickup* pool = new PaintPool(Vector3(stof(object[1]), stof(object[2]), stof(object[3])), START_COLOUR, "0");
	AddPickup(pool);
	CaptureArea* capt = new MultiPaintPool(Vector3(stof(object[4]), stof(object[5]), stof(object[6])), object[7], Vector3(stof(object[8]), stof(object[9]), stof(object[10])), 0);
	AddCaptureArea(capt);
	static_cast<MultiPaintPool*>(capt)->AddPool(static_cast<PaintPool*>(pool));
}

void DataDrivenMap::AddCuboid(vector<std::string> object) {
	Addcuboid(CommonUtils::BuildCuboidObject(object[4], Vector3(stof(object[1]), stof(object[2]), stof(object[3])), Vector3(stof(object[5]), stof(object[6]), stof(object[7])),true, stof(object[8]),true,false,DEFAULT_PHYSICS, DEFAULT_COLOUR));

}

void DataDrivenMap::AddMinionAreas(vector<std::string> object) {

}