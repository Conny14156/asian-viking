#ifdef _DEBUG
#ifndef EngineEditorObjectViewer_HPP
#define EngineEditorObjectViewer_HPP
#include <Windows.h>
#include <vector>
class GameObject;
class EngineObjectListViewer
{
public:
	~EngineObjectListViewer();
	void start();
	void add(GameObject* entity);
	void getSelection();
	void update();


private:
	HWND hWnd;
	std::vector<GameObject*> order;


};


#endif
#endif