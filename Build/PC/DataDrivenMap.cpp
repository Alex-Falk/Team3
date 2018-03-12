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
	float m_AccumTime = 0;
	linenum = 0;
	textID = 0;

	ReadFile();

	for (uint i = 0; i < 10; i++) {
		BuildTextures(GetObjects(lines[i]));
	}

	Buildmap();

	for (uint i = 11; i < 15; i++) {
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
	BuildObject(GetObjects(lines[10]));
}

void DataDrivenMap::BuildObjects() {

	int i = linenum;
	while (i < lines.size())
	{
		vector<string>object = GetObjects(lines[i]);
		BuildObject(object);
		i++;
	}
}


void DataDrivenMap::BuildTextures(vector<std::string> object) {

	if (object[0] == "NEW_TEXTURE") {
		AddTexture(stoi(object[1]), object[2]);
		linenum++;
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
	else if (object[0] == "MAP")			BuildMapDimenions(object);
	else  CoruptedFile(1, linenum);
	linenum++;
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
	AddGameObject(new PaintPool(Vector3(stof(object[1]), stof(object[2]), stof(object[3])), team, object[5]));
}

void DataDrivenMap::AddPickups(vector<std::string> object) {

	if (object[4] == "SPEED_BOOST")		type = SPEED_BOOST;
	if (object[4] == "JUMP_BOOST")		type = JUMP_BOOST;
	AddGameObject(new Pickup(Vector3(stof(object[1]), stof(object[2]), stof(object[3])), type, object[5]));
}

void DataDrivenMap::AddWeaponPickups(vector<std::string> object) {

	if (object[4] == "PAINT_SPRAY")			weapType = PAINT_SPRAY;
	if (object[4] == "PAINT_PISTOL")		weapType = PAINT_PISTOL;
	if (object[4] == "AUTO_PAINT_LAUNCHER")	weapType = AUTO_PAINT_LAUNCHER;
	if (object[4] == "PAINT_ROCKET")		weapType = PAINT_ROCKET;
	AddGameObject(new WeaponPickup(Vector3(stof(object[1]), stof(object[2]), stof(object[3])), weapType, object[5] , stof(object[6])));
}

void DataDrivenMap::AddCaptureAreas(vector<std::string> object) {
	if (Game::Instance()->GetUserID() == 0)
	{
		CaptureArea* ca = new CaptureArea(Vector3(stof(object[1]), stof(object[2]), stof(object[3])), object[4], Vector3(stof(object[5]), stof(object[6]), stof(object[7])), stof(object[8]));
		ca->Physics()->SetInverseMass(stof(object[9]));
		if (object[10] == "TEXTURE") {
			ca->Render()->GetChild()->SetTexture(TextureManager::Instance()->GetTexture(textureID[stoi(object[11])]));
		}
		AddGameObject(ca);
	}
	else
	{
		//-Alex Falk----------------------------------------------------------//
		// Clientside only spawns normal gameobjects, rather than Pickup/Capturearea 
		GameObject * ca = CommonUtils::BuildCuboidObject("CA", Vector3(stof(object[1]), stof(object[2]), stof(object[3])), Vector3(stof(object[5]), stof(object[6]), stof(object[7])));
		ca->SetColour(Colour(stoi(object[8])));
		ca->Physics()->SetInverseMass(stof(object[9]));
		if (object[10] == "TEXTURE") {
			ca->Render()->GetChild()->SetTexture(TextureManager::Instance()->GetTexture(textureID[stoi(object[11])]));
		}
		AddGameObject(ca);
		//--------------------------------------------------------------------//
	}
}

void DataDrivenMap::AddMultiPaintPools(vector<std::string> object) {
	if (Game::Instance()->GetUserID() == 0)
	{
		Pickup* pool = new PaintPool(Vector3(stof(object[1]), stof(object[2]), stof(object[3])), START_COLOUR, object[4]);
		AddGameObject(pool);
		CaptureArea* capt = new MultiPaintPool(Vector3(stof(object[5]), stof(object[6]), stof(object[7])), object[4], Vector3(stof(object[8]), stof(object[9]), stof(object[10])), 0);
		AddGameObject(capt);
		static_cast<MultiPaintPool*>(capt)->AddPool(static_cast<PaintPool*>(pool));

		if (object[11] == "TEXTURE") {
			pool->Render()->GetChild()->SetTexture(TextureManager::Instance()->GetTexture(textureID[stoi(object[12])]));
			capt->Render()->GetChild()->SetTexture(TextureManager::Instance()->GetTexture(textureID[stoi(object[13])]));
		}

	}
	else
	{
		//-Alex Falk----------------------------------------------------------//
		// Clientside only spawns normal gameobjects, rather than Pickup/Capturearea 
		GameObject * pool = CommonUtils::BuildCuboidObject(
			object[4], 
			Vector3(stof(object[1]), stof(object[2]), stof(object[3])), 
			Vector3(3, 0.5, 3),
			true,
			0.0f,
			false,
			false,
			DEFAULT_PHYSICS,
			DEFAULT_COLOUR);

		AddGameObject(pool);

		GameObject * ca = CommonUtils::BuildCuboidObject(
			object[4], 
			Vector3(stof(object[5]), stof(object[6]), stof(object[7])), 
			Vector3(stof(object[8]), stof(object[9]), stof(object[10])),
			true,
			0.0f,
			true, 
			false, 
			DEFAULT_PHYSICS,
			DEFAULT_COLOUR);
		AddGameObject(ca);

		if (object[11] == "TEXTURE") {
			pool->Render()->GetChild()->SetTexture(TextureManager::Instance()->GetTexture(textureID[stoi(object[12])]));
			ca->Render()->GetChild()->SetTexture(TextureManager::Instance()->GetTexture(textureID[stoi(object[13])]));
		}
		//--------------------------------------------------------------------//
	}

}

void DataDrivenMap::AddCuboid(vector<std::string> object) {
	GameObject* cube =  CommonUtils::BuildCuboidObject(
		object[4],
		Vector3(stof(object[1]), stof(object[2]), stof(object[3])), 
		Vector3(stof(object[5]), stof(object[6]), stof(object[7])), 
		true, 
		stof(object[8]), 
		true, 
		false, 
		DEFAULT_PHYSICS, 
		DEFAULT_COLOUR);

	cube->Physics()->SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(stof(object[9]), stof(object[10]), stof(object[11])),1));
	if (object[12] == "TEXTURE") {
		cube->Render()->GetChild()->SetTexture(TextureManager::Instance()->GetTexture(textureID[stoi(object[12])]));
	}
	
	AddGameObject(cube);
}

void DataDrivenMap::AddGround(vector<std::string> object) {
	GameObject* cube = CommonUtils::BuildCuboidObject(object[4], Vector3(stof(object[1]), stof(object[2]), stof(object[3])), Vector3(stof(object[5]), stof(object[6]), stof(object[7])), true, 0, true, false, BIG_NODE, DEFAULT_COLOUR, MATERIALTYPE::Ground);
	cube->Physics()->SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(stof(object[8]), stof(object[9]), stof(object[10])), 1));
	if (object[12] == "TEXTURE") {
		cube->Render()->GetChild()->SetTexture(TextureManager::Instance()->GetTexture(textureID[stoi(object[12])]));
	}
	AddGameObject(cube);
}

void DataDrivenMap::AddMinionAreas(vector<std::string> object) {
	MinionCaptureArea* mca = new MinionCaptureArea(START_COLOUR, to_string(activeCapture), Vector3(stof(object[1]), stof(object[2]), stof(object[3])), Vector3(1.5f, 1.5f, 1.5f), stoi(object[5]));
	AddGameObject(mca);
	if (object[6] == "TEXTURE") {
		mca->Render()->GetChild()->SetTexture(TextureManager::Instance()->GetTexture(textureID[stoi(object[7])]));
	}
}

void DataDrivenMap::AddTexture(uint ID, std::string name) {
	string address = TEXTUREDIR + name;
	CheckTextID(ID);

	if (!TextureManager::Instance()->LoadTexture(textureID[ID], address, GL_REPEAT, GL_NEAREST))
	{
		if (TextureManager::Instance()->LoadTexture(TEXTURETYPE::Ground_Texture, TEXTUREDIR"checkerboard.tga", GL_REPEAT, GL_NEAREST))
		{}
		CoruptedFile(2, linenum);
	}
}

void  DataDrivenMap::CheckTextID(int textID) {
	if (textID == 0) textureID[0] = Item_Texture_1;
	else if (textID == 1) textureID[1] = Item_Texture_2;
	else if (textID == 2) textureID[2] = Item_Texture_3;
	else if (textID == 3) textureID[3] = Item_Texture_4;
	else if (textID == 4) textureID[4] = Item_Texture_5;
	else if (textID == 5) textureID[5] = Item_Texture_6;
	else if (textID == 6) textureID[6] = Item_Texture_7;
	else if (textID == 7) textureID[7] = Item_Texture_8;
	else if (textID == 8) textureID[8] = Item_Texture_9;
	else if (textID == 9) textureID[9] = Item_Texture_10;
}