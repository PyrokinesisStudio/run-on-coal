#include "stdafx.h"
#include "Elements/Camera.h"
#include "Elements/Light.h"
#include "Elements/Scene.h"

ROC::Scene::Scene()
{
    m_elementType = ElementType::SceneElement;

    m_mainCamera = nullptr;
    m_mainLight = nullptr;
}
ROC::Scene::~Scene()
{
}
