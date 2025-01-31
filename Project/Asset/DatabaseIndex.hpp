#ifndef DATABASE_INDEX_HPP
#define DATABASE_INDEX_HPP
#include <Core/String.hpp>

#include "Version.hpp"

class DatabaseIndex
{
	OneVersion modFileVersion = OneVersion(1, 0, 0);

public:
	long long row = 0;
	size_t ID = 0;

	enum class ObjectTypeEnum
	{
		Header = 0,
		Zone,
		Prefab,
		GameObject,
		Quest,
		Item,
		ModelContainer,
		SpriteSheetMap,
		Model,
		DBZone,
		TextureMap,
		PrimitiveInt,
		PrimitiveDouble,
		PrimitiveString,
		StringList,
		Element,
		Rank,
		Language,
		EoF = 254,
		Undefined
	};

	enum class ObjectFlag
	{
		EoF,
		Override,
		AddTo,
		RemoveFrom,
		Modify,
		NoFlag,
		Undefined
	};

	/*
	Type of object to load:
	Zone - load a zonefile
	Prefab - load a prefab
	Gameobject - load a Gameobject
	Quest - load a quest
	Item - load a item
	//*/
	ObjectTypeEnum type = ObjectTypeEnum::Undefined;
	/*
	Flag types:
	EoF - says it is the endof the file - Do not use, it is automatic.
	Override - SourceID - Overrides object with a new one
	AddTo - SourceID - Adds object to source
	RemoveFrom - SourceID - removes object from source.
	Modify - SourceID - Modify flag tells to modifiy that object
	"-" - No flag
	//*/
	ObjectFlag flags = ObjectFlag::NoFlag;
	/*
	ModFile tells from what mod that object comes from
	//*/
	Core::String modFile = "";
	
public:
#pragma region DatabaseIndex
	template<class Archive>
	void load(Archive& ar)
	{
		OneVersion myVersion = OneVersion::EMPTY;
		ar(myVersion);
		// do difference logic here for changes in this file.
		modFileVersion = myVersion;
		ar(row);
		ar(ID);
		ar(type);
		ar(flags);
		ar(modFile);
	}
	template<class Archive>
	void save(Archive& ar) const
	{
		ar(modFileVersion);
		ar(row);
		ar(ID);
		ar(type);
		ar(flags);
		ar(modFile);
	}
#pragma endregion
};

#endif