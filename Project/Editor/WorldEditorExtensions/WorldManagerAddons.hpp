#ifndef WorldManagerAddons_HPP
#define WorldManagerAddons_HPP

#ifdef _EDITOR_
#ifndef _UNITTESTS_
#include <Asset\ModHeader.hpp>

#include <Core\Component\GameObject.h>
#include <Core\EngineModule\EngineModuleManager.hpp>

#include <Graphic\BackgroundSprite.hpp>
#include <Graphic\World\WorldManager.hpp>


class WorldManagerAddon : public IEngineResource<WorldManagerAddon>
{
public:
	ModHeader myModHeader;

	friend class WorldManager;

	WorldManager& myWorldManager;

	WorldManagerAddon();

	Core::String getLoadedMod();
	
	void unloadEditorVariables();

	std::vector<Core::String> getModDependencies(Core::String mod);
	void newMod(Core::String modName, std::vector<Core::String> dependencies, bool createMaster);
	
	void loadMod(Core::String modName);
	bool loadMods(Core::String modName, bool _internal_error = false);
	
	void EditorAddNewZone(Core::String zoneName, Core::String background, Core::String loadingScreen, Core::String loadingScreenMessage, size_t ID, float x, float y);
	void EditorEditZone(Core::String zoneName, Core::String background, Core::String loadingScreen, Core::String loadingScreenMessage, size_t ID, float x, float y);
	void EditorLoadZone(Core::String zoneName, unsigned int ID);
	void EditorRemoveZone();
	
	Core::String EditorSave();
	void EditorSetBackground(std::string textureName);
	void EditorSetBackgroundSize(int x, int y);
	std::pair<std::pair<Core::String, size_t>, Database::Prefab> EditorAddGameObjectToZone(Asset::Prefab& prefab, GameObject* go);
	
	void RemoveGameObjectFromZone(GameObject* go);
	void EditorFlagGameObjectForEdit(GameObject* go);


	const ResourceType& getType()
	{
		return type;
	}

	//Item1 - BackgroundPath
	//Item2 - LoadingScreenPath
	//Item3 - LoadingScreenMessage
	std::vector<Core::String> getExtraZoneInfo(Core::String modName, size_t id);

	std::vector<Database::Zone> getAllDbZones();

	size_t EditorGetValidID();
	size_t ID = 1;

	Database::Zone* currentDBZone;
	BackgroundSprite unloadingbg;
};



#endif
#endif
#endif