/*                      (.      ,(,                                        
                     ,(*       (     ,#/  (%    ,%*.                   
                 .%(        ,&*#     *( .* .&/ /#%& ,,    ,/             
               ,@*,,.      %(% %.%#,#%/ /@, /,*(#%(,.&,                  
               #   ,(&      %,,% ((    ##&&/,      #.##      .         
       #/    (%(    .%   ,,(%. ,         ,.         %(#*#.  #%&@,        
        . ,#(/,     #%#.#.%,                           ,(&,, %/.       
          ,/,        .**,.                               (#,   ,%        
      .#%  ,,         ,,         #*                       ,#   /&@%%.    
      %,*%%*                   ##  ##    %@@@@@@@@@@@@@@@@@*##   /#.%    
      ,*       %, @*          .,&%%&,.   %%(%%%%%%%%%%%%%%%.*#.    #.    
     ,*        %,  #*           %%%%     %%%%             ,#       *(    
    ,%*,*#     %,%@*#&          %%%%     %%%%                     ,..    
    .,., ,,    %,%*&**&         %%%%     %%%%                      .#    
     ,,#,#.    %,%, &&*@        %%%%     %%%%                    ,%##  ,%
      #        %,%,  *#.@*      %%%%     %%%%         ,@#          %%.,&*
    %(#.%.     %,%,   #@ &*     %%%%     %%.,,,,,,,,,,,. @#      ,@,     
   ,, % %,     %,%,    *& @#    %%%%     %%(%%%%%%%%%%%(*#.       .,,#   
 .&,(. ,.      %,%,     *@ ##   %%%%     %%%%         .#         %/*#   
  *#  *&       %,%,      .@.#@  %%%%     %%%%                        /,  
   .#/         %,%,       .@..@.%%%%     %%%%                     /#*    
  %%((         %,%,         @#.@@%%%     %%%%                     ,*,    
  (            %,%,          #* @%%%     %%%%                     (@(    
    #.         %,%,           #&  %%     %%%%                     *%%,   
 (* %.       #@%.(@@.          *@@@%   #@@%%@@#                  .#      
(              #(@*                      @#*&                   */
#include "DataDrivenMap.h"
#include "WeaponPickup.h"
#include "PaintPool.h"
#include "Game.h"
#include "MultiPaintPool.h"
#include <nclgl\Lexer.h>
#include <ncltech\CommonUtils.h>
#include "MinionCaptureArea.h"


//--------------------------------------------------------------------------------------------//
// Initialisation and Cleanup
//--------------------------------------------------------------------------------------------//
void DataDrivenMap::OnInitializeScene()
{
	m_AccumTime = 0;
	activePickups = 0;
	activeCapture = 0;
	linenum = 0;

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
	try {
		std::ifstream ifs("../../Data/Maps/"+fileName+".txt");
		std::string file((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));

	lines = GetLines(file);
	ifs.close();
	}
	catch (const ifstream::failure& e) {
		CoruptedFile(0,0);
	}
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
	linenum++;
	if (object[0] == "CAPTURE_AREA")		AddCaptureAreas(object);
	else if (object[0] == "CUBE")			AddCuboid(object);
	else if (object[0] == "PICKUP")			AddPickups(object);
	else if (object[0] == "WEAPON_PICKUP")	AddWeaponPickups(object);
	else if (object[0] == "PAINT_POOL")		AddPaintPools(object);
	else if (object[0] == "MULTI_POOL")		AddMultiPaintPools(object);
	else if (object[0] == "MINION_AREA")	AddMinionAreas(object);
	else if (object[0] == "GROUND")			AddGround(object);
	else if (object[0] == "SPAWN_LOC")		SetSpawnLocation(object);
	else if (object[0] == "MAP")			BuildMapDimenions(object);
	else  CoruptedFile(1, linenum);
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

void DataDrivenMap::BuildMapDimenions(vector<std::string> object) {
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
	AddPickup(new PaintPool(Vector3(stof(object[1]), stof(object[2]), stof(object[3])), team, to_string(activePickups)));
	activePickups++;
}

void DataDrivenMap::AddPickups(vector<std::string> object) {

	if (object[4] == "SPEED_BOOST")		type = SPEED_BOOST;
	if (object[4] == "JUMP_BOOST")		type = JUMP_BOOST;
	AddPickup(new Pickup(Vector3(stof(object[1]), stof(object[2]), stof(object[3])), type, to_string(activePickups)));
	activePickups++;
}

void DataDrivenMap::AddWeaponPickups(vector<std::string> object) {

	if (object[4] == "PAINT_SPRAY")			weapType = PAINT_SPRAY;
	if (object[4] == "PAINT_PISTOL")		weapType = PAINT_PISTOL;
	if (object[4] == "AUTO_PAINT_LAUNCHER")	weapType = AUTO_PAINT_LAUNCHER;
	if (object[4] == "PAINT_ROCKET")		weapType = PAINT_ROCKET;
	AddPickup(new WeaponPickup(Vector3(stof(object[1]), stof(object[2]), stof(object[3])), weapType, to_string(activePickups), stof(object[6])));
	activePickups++;
}

void DataDrivenMap::AddCaptureAreas(vector<std::string> object) {
	AddCaptureArea(new CaptureArea(Vector3(stof(object[1]), stof(object[2]), stof(object[3])), to_string(activeCapture), Vector3(stof(object[5]), stof(object[6]), stof(object[7])), stof(object[8])));
	activeCapture++;
}

void DataDrivenMap::AddMultiPaintPools(vector<std::string> object) {

	Pickup* pool = new PaintPool(Vector3(stof(object[1]), stof(object[2]), stof(object[3])), START_COLOUR, to_string(activePickups));
	AddPickup(pool);
	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Checker_Board, TEXTUREDIR"checkerboard.tga", GL_REPEAT, GL_NEAREST))
		CoruptedFile(2, linenum);
	activePickups++;
	CaptureArea* capt = new MultiPaintPool(Vector3(stof(object[5]), stof(object[6]), stof(object[7])), to_string(activeCapture), Vector3(stof(object[8]), stof(object[9]), stof(object[10])), 0);
	AddCaptureArea(capt);
	activeCapture++;
	static_cast<MultiPaintPool*>(capt)->AddPool(static_cast<PaintPool*>(pool));
}

void DataDrivenMap::AddCuboid(vector<std::string> object) {
	GameObject* cube =  CommonUtils::BuildCuboidObject(object[4], Vector3(stof(object[1]), stof(object[2]), stof(object[3])), Vector3(stof(object[5]), stof(object[6]), stof(object[7])), true, stof(object[8]), true, false, DEFAULT_PHYSICS, DEFAULT_COLOUR);
	cube->Physics()->SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(stof(object[9]), stof(object[10]), stof(object[11])),1));
	Addcuboid(cube);
}

void DataDrivenMap::AddGround(vector<std::string> object) {
	GameObject* cube = CommonUtils::BuildCuboidObject(object[4], Vector3(stof(object[1]), stof(object[2]), stof(object[3])), Vector3(stof(object[5]), stof(object[6]), stof(object[7])), true, 0, true, false, BIG_NODE, DEFAULT_COLOUR, MATERIALTYPE::Ground);
	cube->Physics()->SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(stof(object[8]), stof(object[9]), stof(object[10])), 1));
	Addcuboid(cube);
}

void DataDrivenMap::AddMinionAreas(vector<std::string> object) {
	AddCaptureArea(new MinionCaptureArea(START_COLOUR, to_string(activeCapture), Vector3(stof(object[1]), stof(object[2]), stof(object[3])), Vector3(1.5f,1.5f,1.5f), stoi(object[5])));
	activeCapture++;
}

GLuint DataDrivenMap::AddTexture(std::string name) {
	string address = "../../Data/Textures/" + name;
	GLuint texture = SOIL_load_OGL_texture(address.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT);
	if (!texture)
	{
		string address = "../../Data/Textures/checkerboard.tga";
		texture = SOIL_load_OGL_texture(address.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT);
	}
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}