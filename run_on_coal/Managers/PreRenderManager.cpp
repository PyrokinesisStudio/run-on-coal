#include "stdafx.h"

#include "Managers/PreRenderManager.h"
#include "Core/Core.h"
#include "Elements/Model/Model.h"
#include "Lua/LuaArguments.h"
#include "Utils/TreeNode.h"

#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"

ROC::PreRenderManager::PreRenderManager(Core *f_core)
{
    m_core = f_core;
    m_luaArguments = new LuaArguments();
    m_callback = nullptr;
    m_modelTreeRoot = new TreeNode(nullptr);
    m_modelToNodeMapEnd = m_modelToNodeMap.end();
}
ROC::PreRenderManager::~PreRenderManager()
{
    auto &l_rootNodes = m_modelTreeRoot->GetChildren();
    m_nodeStack.insert(m_nodeStack.end(), l_rootNodes.rbegin(), l_rootNodes.rend());
    while(!m_nodeStack.empty())
    {
        TreeNode *l_node = m_nodeStack.back();
        m_nodeStack.pop_back();

        auto &l_childrenVector = l_node->GetChildren();
        m_nodeStack.insert(m_nodeStack.end(), l_childrenVector.rbegin(), l_childrenVector.rend());
        delete l_node;
    }
    delete m_modelTreeRoot;
    delete m_luaArguments;
}

void ROC::PreRenderManager::AddModel(Model *f_model)
{
    TreeNode *l_node = new TreeNode(f_model);
    m_modelTreeRoot->AddChild(l_node);
    m_modelToNodeMap.insert(std::make_pair(f_model, l_node));
    m_modelToNodeMapEnd = m_modelToNodeMap.end();
}
void ROC::PreRenderManager::RemoveModel(Model *f_model)
{
    auto l_modelIter = m_modelToNodeMap.find(f_model);
    if(l_modelIter != m_modelToNodeMapEnd)
    {
        TreeNode *l_node = l_modelIter->second;
        TreeNode *l_parentNode = l_node->GetParent();
        if(l_parentNode) l_parentNode->RemoveChild(l_node);
        else m_modelTreeRoot->RemoveChild(l_node);

        for(auto iter : l_node->GetChildren())
        {
            iter->SetParent(nullptr);
            m_modelTreeRoot->AddChild(iter);
        }

        m_modelToNodeMap.erase(l_modelIter);
        m_modelToNodeMapEnd = m_modelToNodeMap.end();
        delete l_node;
    }
}

void ROC::PreRenderManager::AddLink(Model *f_model, Model *f_parent)
{
    auto l_modelIter = m_modelToNodeMap.find(f_model);
    if(l_modelIter != m_modelToNodeMapEnd)
    {
        auto l_parentIter = m_modelToNodeMap.find(f_parent);
        if(l_parentIter != m_modelToNodeMapEnd)
        {
            TreeNode *l_modelNode = l_modelIter->second;
            TreeNode *l_parentNode = l_parentIter->second;
            if(!l_parentNode->HasChild(l_modelNode))
            {
                TreeNode *l_modelParentNode = l_modelNode->GetParent();
                if(l_modelParentNode) l_modelParentNode->RemoveChild(l_modelNode);
                else m_modelTreeRoot->RemoveChild(l_modelNode);

                l_modelNode->SetParent(l_parentNode);
                l_parentNode->AddChild(l_modelNode);
            }
        }
    }
}
void ROC::PreRenderManager::RemoveLink(Model *f_model)
{
    auto l_modelIter = m_modelToNodeMap.find(f_model);
    if(l_modelIter != m_modelToNodeMapEnd)
    {
        TreeNode *l_node = l_modelIter->second;
        TreeNode *l_parentNode = l_node->GetParent();
        if(l_parentNode)
        {
            l_parentNode->RemoveChild(l_node);
            m_modelTreeRoot->AddChild(l_node);
            l_node->SetParent(nullptr);
        }
    }
}

void ROC::PreRenderManager::DoPulse_S1()
{
    if(m_callback) (*m_callback)();
    m_core->GetLuaManager()->GetEventManager()->CallEvent(EventManager::EME_onPreRender, m_luaArguments);

    auto &l_rootNodes = m_modelTreeRoot->GetChildren();
    m_nodeStack.insert(m_nodeStack.end(), l_rootNodes.rbegin(), l_rootNodes.rend());
    while(!m_nodeStack.empty())
    {
        TreeNode *l_current = m_nodeStack.back();
        Model *l_model = reinterpret_cast<Model*>(l_current->GetPointer());

        if(!l_model->HasCollision()) l_model->Update(Model::MUS_Matrix);
        l_model->Update(Model::MUS_Animation);
        l_model->Update(Model::MUS_SkeletonCollisionStatic);

        m_nodeStack.pop_back();
        auto &l_nodeChildren = l_current->GetChildren();
        m_nodeStack.insert(m_nodeStack.end(), l_nodeChildren.rbegin(), l_nodeChildren.rend());
    }
}
void ROC::PreRenderManager::DoPulse_S2()
{
    auto &l_rootNodes = m_modelTreeRoot->GetChildren();
    m_nodeStack.insert(m_nodeStack.end(), l_rootNodes.rbegin(), l_rootNodes.rend());
    while(!m_nodeStack.empty())
    {
        TreeNode *l_current = m_nodeStack.back();
        Model *l_model = reinterpret_cast<Model*>(l_current->GetPointer());

        l_model->Update(l_model->HasCollision() ? Model::MUS_Collision : Model::MUS_Matrix);
        l_model->Update(Model::MUS_SkeletonCollisionDynamic);

        m_nodeStack.pop_back();
        auto &l_nodeChildren = l_current->GetChildren();
        m_nodeStack.insert(m_nodeStack.end(), l_nodeChildren.rbegin(), l_nodeChildren.rend());
    }
}
