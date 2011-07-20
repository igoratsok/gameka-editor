/***********************************************

Gameka: A game development tool for non-programmers
Copyright (C) 2011 Igor Augusto de Faria Costa

This software is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

************************************************/




#include "gamedata.h"


int GameData::MAGIC_NUMBER = 0xAFACABBE;
GameData* GameData::singleton = NULL;

GameData::GameData()
{
    gameName = NULL;
    tileSetList = new std::vector<TileSet*>();
    mapList = new std::vector<Map*>();
    objectList = new std::vector<GameObject*>();
    animationDataList = new std::vector<AnimationData*>();
    backgroundList = new std::vector<Background*>();
    procedureList = new std::vector<ProcedureData*>();
    variableList = new std::vector<VariableData*>();
    editingMap = NULL;

    skillList = Skill::getEditorSkills();

    titleScreenMusic = std::string("");
    optionsScreenMusic = std::string("");
    gameOverScreenMusic = std::string("");

    loopGameOverScreenMusic = -1;
    loopTitleScreenMusic = -1;
    loopOptionsScreenMusic = -1;
    idFirstMap = 0;

    deathAction = 0;
    gameOverAction = 0;
    looseLifes = true;
    totalLives = 5;

    procedureGlobalAlways = new ProcedureData();
    procedureGlobalAlways->name = QString::fromUtf8("Sempre").toStdString();
    procedureGlobalAlways->nameEditable = false;
    procedureGlobalStart = new ProcedureData();
    procedureGlobalStart->name = QString::fromUtf8("Início do jogo").toStdString();
    procedureGlobalStart->nameEditable = false;
    procedureGlobalEnd = new ProcedureData();
    procedureGlobalEnd->name = QString::fromUtf8("Fim do jogo").toStdString();
    procedureGlobalEnd->nameEditable = false;

}

GameData::~GameData()
{
}

GameData* GameData::getInstance() {
    if(singleton == NULL) {
        singleton = new GameData();
    }

    return singleton;
}



std::string GameData::getProjectDirectory() {
    int posicaoUltimaBarra;

    posicaoUltimaBarra = filePath.find_last_of("/");

    return filePath.substr(0, posicaoUltimaBarra);
}



void GameData::erase() {
    if(gameName != NULL) {
        delete gameName;
    }

    gameName = NULL;

    resWidth = 0;
    resHeight = 0;
}



TileSet* GameData::searchTilesetById(int id) {
    for(std::vector<TileSet*>::iterator it = tileSetList->begin(); it != tileSetList->end(); ++it) {
        TileSet *tileset = *it;

        if(tileset->id == id) {
            return tileset;
        }
    }

    return NULL;
}

GameObject* GameData::searchGameObjectById(int id) {
    for(std::vector<GameObject*>::iterator it = objectList->begin(); it != objectList->end(); ++it) {
        GameObject *gameObject = *it;

        if(gameObject->id == id) {
            return gameObject;
        }
    }

    return NULL;
}

AnimationData* GameData::searchAnimationDataById(int id) {
    for(std::vector<AnimationData*>::iterator it = animationDataList->begin(); it != animationDataList->end(); ++it) {
        AnimationData *animationData = *it;

        if(animationData->id == id) {
            return animationData;
        }
    }

    return NULL;
}

Background* GameData::searchBackgroundById(int id) {
    for(std::vector<Background*>::iterator it = backgroundList->begin(); it != backgroundList->end(); ++it) {
        Background *bg = *it;

        if(bg->id == id) {
            return bg;
        }
    }

    return NULL;
}

Skill* GameData::searchSkillById(int id) {
    for(std::vector<Skill*>::iterator it = skillList->begin(); it != skillList->end(); ++it) {
        Skill *skill = *it;

        if(skill->id == id) {
            return skill;
        }
    }

    return NULL;
}

ProcedureData* GameData::getProcedureById(int id) {
    for(std::vector<ProcedureData*>::iterator it = procedureList->begin(); it != procedureList->end(); ++it) {
        ProcedureData *procedureData = *it;

        if(procedureData->id == id) {
            return procedureData;
        }
    }

    return NULL;
}

VariableData* GameData::getVariableById(int id) {
    for(std::vector<VariableData*>::iterator it = variableList->begin(); it != variableList->end(); ++it) {
        VariableData *variableData = *it;

        if(variableData->id == id) {
            return variableData;
        }
    }

    return NULL;
}

void GameData::addTileset(TileSet *tileSet) {
    tileSet->id = generateNewTilesetId();

    tileSetList->push_back(tileSet);
}

void GameData::addAnimation(AnimationData *animationData) {
    animationData->id = generateNewAnimationId();

    animationDataList->push_back(animationData);
}

void GameData::addGameObject(GameObject *gameObject) {
    gameObject->id = generateNewGameObjectId();

    objectList->push_back(gameObject);
}

void GameData::addBackground(Background *background) {
    background->id = generateNewBackgroundId();

    backgroundList->push_back(background);
}

void GameData::addProcedure(ProcedureData *procedureData) {
    procedureData->id = generateNewProcedureId();

    std::stringstream ss;
    ss << "Procedimento " << procedureData->id;

    procedureData->name = ss.str();

    procedureList->push_back(procedureData);
}

void GameData::addVariable(VariableData *variableData) {
    variableData->id = generateNewVariableId();

    variableList->push_back(variableData);
}

int GameData::generateNewTilesetId() {
    int id = 0;
    int itemId;
    TileSet* tileSetItem;

    if(!tileSetList->empty()) {
        for(std::vector<TileSet*>::iterator it = tileSetList->begin(); it != tileSetList->end(); ++it) {
            tileSetItem = *it;
            itemId = tileSetItem->id;
            if(itemId > id) id = itemId;
        }
    }


    return (id + 1);
}

int GameData::generateNewAnimationId() {
    int id = 0;
    int itemId;

    if(!animationDataList->empty()) {
        for(std::vector<AnimationData*>::iterator it = animationDataList->begin(); it != animationDataList->end(); ++it) {
            AnimationData *animationData = *it;
            itemId = animationData->id;
            if(itemId > id) id = itemId;
        }
    }

    return (id + 1);
}

int GameData::generateNewGameObjectId() {
    int id = 0;
    int itemId;
    GameObject* gameObjectItem;

    if(!objectList->empty()) {
        for(std::vector<GameObject*>::iterator it = objectList->begin(); it != objectList->end(); ++it) {
            gameObjectItem = *it;
            itemId = gameObjectItem->id;
            if(itemId > id) id = itemId;
        }
    }

    return (id + 1);
}

int GameData::generateNewBackgroundId() {
    int id = 0;
    int bgId;
    Background *bgItem;

    if(!backgroundList->empty()) {
        for(std::vector<Background*>::iterator it = backgroundList->begin(); it != backgroundList->end(); ++it) {
            bgItem = *it;
            bgId = bgItem->id;
            if(bgId > id) id = bgId;
        }
    }

    return (id + 1);
}

int GameData::generateNewProcedureId() {
    int id = 0;
    int procedureId;
    ProcedureData *procedureItem;

    if(!procedureList->empty()) {
        for(std::vector<ProcedureData*>::iterator it = procedureList->begin(); it != procedureList->end(); ++it) {
            procedureItem = *it;
            procedureId = procedureItem->id;
            if(procedureId > id) id = procedureId;
        }
    }

    return (id + 1);
}

int GameData::generateNewVariableId() {
    int id = 0;
    int bgId;
    VariableData *variableItem;

    if(!variableList->empty()) {
        for(std::vector<VariableData*>::iterator it = variableList->begin(); it != variableList->end(); ++it) {
            variableItem = *it;
            bgId = variableItem->id;
            if(bgId > id) id = bgId;
        }
    }

    return (id + 1);
}


int GameData::generateNewMapId() {
    int id;

    id = generateNewMapIdPercorreArvore(mapList, 0);

    return id;
}

int GameData::generateNewMapIdPercorreArvore(std::vector<Map*> *mapList, int id) {
    int itemId;
    Map* mapItem;
    itemId = 0;

    if(!mapList->empty()) {
        for(std::vector<Map*>::iterator it = mapList->begin(); it != mapList->end(); ++it) {
            mapItem = *it;
            if(mapItem->isFolder()) {
                id = generateNewMapIdPercorreArvore(mapItem->filhos, id);
            } else {
                itemId = mapItem->id;
                if(itemId > id) id = itemId;
            }

        }
    }

    return (id + 1);
}


void GameData::setTitleScreenFile(std::string fileName) {
    titleScreenData.backgroundFile = fileName;
}

void GameData::setOptionsScreenFile(std::string fileName) {
    optionsScreenFile = fileName;
}

void GameData::setGameOverScreenFile(std::string fileName) {
    gameoverScreenFile = fileName;
}
