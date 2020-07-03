#include "EntityDisplayApp.h"
#include "FixWindows.h"
#include <windows.h>
#include <iostream>

EntityDisplayApp::EntityDisplayApp(int screenWidth, int screenHeight) : m_screenWidth(screenWidth), m_screenHeight(screenHeight) {

}

EntityDisplayApp::~EntityDisplayApp() {

}

bool EntityDisplayApp::Startup() {

	fileHandleData = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"fileHandleData");
	fileHandleSize = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"fileHandleSize");

	InitWindow(m_screenWidth, m_screenHeight, "EntityDisplayApp");
	SetTargetFPS(60);

	size = (int*)MapViewOfFile(fileHandleSize, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int));

	entities = (Entity*)MapViewOfFile(fileHandleData, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(Entity) * *size);

	for (int i = 0; i < *size; i++)
	{
		m_entities.push_back(entities[i]);
	}

	return true;
}

void EntityDisplayApp::Shutdown() {

	CloseWindow();        // Close window and OpenGL context

	CloseHandle(fileHandleData);
	CloseHandle(fileHandleSize);
	UnmapViewOfFile(entities);
	UnmapViewOfFile(size);

	delete &m_entities;
}

void EntityDisplayApp::Update(float deltaTime) {
	
	for (int i = 0; i < *size; i++)
	{
		m_entities[i] = entities[i];
	}
}

void EntityDisplayApp::Draw() {
	BeginDrawing();

	ClearBackground(RAYWHITE);

	// draw entities
	for (auto& entity : m_entities) {
		DrawRectanglePro(
			Rectangle{ entity.x, entity.y, entity.size, entity.size }, // rectangle
			Vector2{ entity.size / 2, entity.size / 2 }, // origin
			entity.rotation,
			Color{ entity.r, entity.g, entity.b, 255 });
	}

	// output some text, uses the last used colour
	DrawText("Press ESC to quit", 630, 15, 12, LIGHTGRAY);

	EndDrawing();
}