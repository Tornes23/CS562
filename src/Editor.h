#pragma once
union SDL_Event;

class EditorClass
{
public:
	//Singleton stuff
	EditorClass(EditorClass const&) = delete;
	void operator=(EditorClass const&) = delete;
	static EditorClass& GetInstance()
	{
		static EditorClass instance;
		return instance;
	}

	//member functions
	void Initialize();
	void Render();
	void StartFrame();
	void HandleEvent(SDL_Event* event);
	void ShutDown();

private:
	EditorClass() {}
};

#define Editor (EditorClass::GetInstance())