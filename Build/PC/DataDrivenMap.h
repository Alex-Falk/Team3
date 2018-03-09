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
(              #(@*                      @#*&                    *,      
*/
#pragma once
#include <string>																				
#include <fstream>
#include <vector>																				
#include <iterator>	
#include "Map.h"


class DataDrivenMap :public Map
{
private:
	float m_AccumTime;
	uint activePickups;
	uint activeCapture;
	uint numemptyline;
	Map* map;
	string fileName = "Map4";
	vector<string> lines;

	vector<Pickup> pickups;
	vector<CaptureArea> captAreas;
	Colour team;
	PickupType type;
	WeaponType weapType;
	Vector3 sPotision[4];

	// Building Functions
	vector<string> GetLines(std::string file);
	vector<string> GetObjects(std::string line);

	void ReadFile();
	void Buildmap();
	void BuildObjects();
	void BuildObject(vector<std::string> object);
	void BuildMapDimenions(vector<std::string> object);
	void AddGround(vector<std::string> object);
	void AddCuboid(vector<std::string> object);
	void SetSpawnLocation(vector<std::string> object);
	void AddPaintPools(vector<std::string> object);
	void AddPickups(vector<std::string> object);
	void AddWeaponPickups(vector<std::string> object);
	void AddCaptureAreas(vector<std::string> object);
	void AddMultiPaintPools(vector<std::string> object);
	void AddMinionAreas(vector<std::string> object);
	inline void CoruptedFile(uint errorNumber, uint errorLine){ GUIsystem::Instance()->MapProblem(errorNumber, errorLine); }

public:
	//--------------------------------------------------------------------------------------------//
	// Initialisation and Cleanup
	//--------------------------------------------------------------------------------------------//
	DataDrivenMap(const std::string& friendly_name) :
		Map(friendly_name)
	{
	}

	~DataDrivenMap() {

	}

	virtual void OnInitializeScene() override;
	virtual void SetSpawnLocations() override;
	virtual void AddObjects() override;

	//--------------------------------------------------------------------------------------------//
	// Special Object udpates (e.g. Pickups)
	//--------------------------------------------------------------------------------------------//
	virtual void OnUpdateScene(float dt) override;

	//--------------------------------------------------------------------------------------------//
	// Utility. TODO: can probably (re)move this
	//--------------------------------------------------------------------------------------------//
	void onButtonClicked();

};



















