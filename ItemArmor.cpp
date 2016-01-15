#include "ItemArmor.h"
#include "Engine.h"
#include "InventoryState.h"

ItemArmor::ItemArmor(){
    this->hoveringOver = false;
    this->armorSpecials.clear();
    this->armorStats.clear();
    this->armorPiece = 0;
}

bool ItemArmor::getHoveringOver(){
    return this->hoveringOver;
}
string ItemArmor::getItemName(){
    return this->armorName;
}
double ItemArmor::getItemStat(int stat){
    return this->armorStats[stat];
}
vector<double> ItemArmor::getItemStats(){
    return this->armorStats;
}
double ItemArmor::getItemSpecial(int special){
    return this->armorSpecials[special];
}
vector<double> ItemArmor::getItemSpecials(){
    return this->armorSpecials;
}
int ItemArmor::getItemRarity(){
    return this->armorRarity;
}
int ItemArmor::getItemPiece(){
    return this->armorPiece;
}
string ItemArmor::getArmorPieceAsString(int armorPiece){
    switch(armorPiece){
        case armorHelmet:
            return "Helmet"; break;

        case armorChestArmor:
            return "Chest Armor"; break;

        case armorGloves:
            return "Gloves"; break;

        case armorPants:
            return "Pants"; break;

        case armorBoots:
            return "Boots"; break;

        case armorCape:
            return "Cape"; break;
    }
}

void ItemArmor::setArmorStats(vector<double> armorStats, string name, int armorPiece){
    this->armorName = name;
    this->armorStats = armorStats;
    this->armorPiece = armorPiece;
}

void ItemArmor::setArmorSpecials(vector<double> armorSpecials){
    this->armorSpecials = armorSpecials;
}

void ItemArmor::setArmorParts(int rarity, int type, int prefix, int suffix){
    this->armorRarity = rarity;
    this->armorType = type;
    this->armorPrefix = prefix;
    this->armorSuffix = suffix;
}

void ItemArmor::update(){
    if(this->entityWorldPosition != worldPosition && this->usesWorldPosition){
        return;
    }
    this->hoveringOver = false;
    if(checkCollision(this->posX, this->posY, mouseX, mouseY, this->width, this->height, 0, 0)){
        this->hoveringOver = true;
        if(mouseButtonLeftClick){
            if(playerList[0]->getPlayerEquippedArmor(this->armorPiece) == this->entityId && playerList[0]->getPlayerHasArmorEquipped(this->armorPiece)){
                playerList[0]->unequipArmor(this->armorPiece);
            }else{
                playerList[0]->equipArmor(this->entityId);
            }
            updateInventoryPlacement();
        }
    }
}

void ItemArmor::draw(){
    if(this->entityWorldPosition != worldPosition && this->usesWorldPosition){
        return;
    }

    al_draw_filled_rectangle(this->posX, this->posY, this->posX+this->width, this->posY+this->height, lootSystem.getRarityColor(this->armorRarity));
    if(this->hoveringOver){
        drawItemIDInformationBox = this->entityId;
    }
}

void ItemArmor::drawInformationBox(){
    ALLEGRO_COLOR rarityColor = lootSystem.getRarityColor(this->armorRarity);

    double boxWidth = 450, boxHeight = 250;

    al_draw_filled_rectangle(mouseX, mouseY-boxHeight, mouseX+boxWidth, mouseY, al_map_rgb(20, 20, 20));
    al_draw_text(defaultFont, rarityColor, mouseX, mouseY-boxHeight, 0, this->armorName.c_str());
    al_draw_textf(smallFont, al_map_rgb(235, 190, 65), mouseX, mouseY-boxHeight+23, 0,  "Item Level: %.2f", this->itemLevel);
    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-boxHeight+40, 0, "Armor Type: %s", this->getArmorPieceAsString(this->armorPiece).c_str());
    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-boxHeight+55, 0, "Armor: %.2f", this->armorStats[armorValue]);
    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-boxHeight+70, 0, "Encumbrance: %.2f kg", this->armorStats[armorEncumbranceRating]);

    int specialsDrawn = 0;
    for(int i = 0; i < this->armorSpecials.size(); i++){
        if(this->armorSpecials[i] != 0){
            switch(i){
                case armorAdditionalMovementSpeed:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Additional Movement Speed: %.2f tiles/s", this->armorSpecials[armorAdditionalMovementSpeed]/tileSize); break;
                case armorDamageReflection:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Damage Reflection: %.2f%%", this->armorSpecials[armorDamageReflection]*100); break;
                case armorAdditionalLife:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Additional Life: %.2f", this->armorSpecials[armorAdditionalLife]); break;
                case armorRetributionDamage:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Retribution Damage: %.2f", this->armorSpecials[armorRetributionDamage]); break;
                case armorSlowAuraStrength:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Slow Aura Strength: %.2f", this->armorSpecials[armorSlowAuraStrength]); break;
                case armorFireAuraStrength:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Fire Aura Strength: %.2f tiles", this->armorSpecials[armorFireAuraStrength]); break;
            }
            specialsDrawn++;
        }
    }

    if(compareItems){
        double smallBoxWidth = 250, smallBoxHeight = 180;

        double offsetX = boxWidth+15;

        int pW = playerList[0]->getPlayerEquippedArmor(this->armorPiece);

        ALLEGRO_COLOR equippedRarityColor = lootSystem.getRarityColor(itemList[pW]->getItemRarity());

        al_draw_filled_rectangle(mouseX+offsetX, mouseY-smallBoxHeight, mouseX+offsetX+smallBoxWidth, mouseY, al_map_rgb(10, 10, 10));
        if(playerList[0]->getPlayerHasArmorEquipped(this->armorPiece)){
            al_draw_text(mediumFont, equippedRarityColor, mouseX+offsetX, mouseY-smallBoxHeight, 0, itemList[pW]->getItemName().c_str());
            al_draw_textf(tinyFont, al_map_rgb(235, 190, 65), mouseX+offsetX, mouseY-smallBoxHeight+23, 0, "Item Level: %.2f", itemList[pW]->getItemLevel());
            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-smallBoxHeight+40, 0, "Armor Type: %s", this->getArmorPieceAsString(itemList[pW]->getItemPiece()).c_str());
            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-smallBoxHeight+50, 0, "Armor: %.2f", itemList[pW]->getItemStat(armorValue));
            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-smallBoxHeight+60, 0, "Encumbrance: %.2f kg", itemList[pW]->getItemStat(armorEncumbranceRating));

            al_draw_textf(smallFont, rarityColor, mouseX+boxWidth, mouseY-boxHeight+55, ALLEGRO_ALIGN_RIGHT, "Armor: %.2f", this->armorStats[armorValue]-itemList[pW]->getItemStat(armorValue));
            al_draw_textf(smallFont, rarityColor, mouseX+boxWidth, mouseY-boxHeight+70, ALLEGRO_ALIGN_RIGHT, "Encumbrance: %.2f kg", this->armorStats[armorEncumbranceRating]-itemList[pW]->getItemStat(armorEncumbranceRating));

            int specialsDrawn = 0;
            for(int i = 0; i < this->armorSpecials.size(); i++){
                if(itemList[pW]->getItemSpecial(i) != 0){
                    switch(i){
                        case armorAdditionalMovementSpeed:
                            al_draw_textf(tinyFont, rarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*15), 0, "Additional Movement Speed: %.2f tiles/s", itemList[pW]->getItemSpecial(armorAdditionalMovementSpeed)/tileSize); break;
                        case armorDamageReflection:
                            al_draw_textf(tinyFont, rarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*15), 0, "Damage Reflection: %.2f%%", itemList[pW]->getItemSpecial(armorDamageReflection)*100); break;
                        case armorAdditionalLife:
                            al_draw_textf(tinyFont, rarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*15), 0, "Additional Life: %.2f", itemList[pW]->getItemSpecial(armorAdditionalLife)); break;
                        case armorRetributionDamage:
                            al_draw_textf(tinyFont, rarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*15), 0, "Retribution Damage: %.2f", itemList[pW]->getItemSpecial(armorRetributionDamage)); break;
                        case armorSlowAuraStrength:
                            al_draw_textf(tinyFont, rarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*15), 0, "Slow Aura Strength: %.2f", itemList[pW]->getItemSpecial(armorSlowAuraStrength)); break;
                        case armorFireAuraStrength:
                            al_draw_textf(tinyFont, rarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*15), 0, "Fire Aura Strength: %.2f tiles", itemList[pW]->getItemSpecial(armorFireAuraStrength)); break;
                    }
                    specialsDrawn++;
                }
            }
        }else{
            al_draw_text(mediumFont, al_map_rgb(255, 255, 255), mouseX+offsetX, mouseY-smallBoxHeight, 0, "Player has no armor of this type equipped");
        }
    }
}
