/*
================================
 scenemanager.h
 
 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#pragma warning( disable : 4005 )

#include <Windows.h>
#include <memory>
 
class D3DBase;
class Camera;
class Model; 
class Shader; 
class Light;

/*
================================
 
 SceneManager
 
 Main class for rendering the scene.
 Contains pointers to all the objects in the scene.

================================
*/
class SceneManager
{
public:
	SceneManager();
	SceneManager(SceneManager const&);
	~SceneManager();

	bool init(int, int, HWND);
	bool update();
	bool render();

private:	
    std::unique_ptr<D3DBase> m_d3d;
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Model> m_model;
    std::unique_ptr<Shader> m_shader;td::unique_ptr<Light> m_light;
	
	void renderScene();
};

