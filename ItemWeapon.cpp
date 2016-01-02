#include "ItemWeapon.h"
#include "Engine.h"

LootSystem lootSystem;

ItemWeapon::ItemWeapon(){
    this->hoveringOver = false;
    this->weaponSpecials.clear();
    this->weaponStats.clear();
}

bool ItemWeapon::getHoveringOver(){
    return this->hoveringOver;
}
string ItemWeapon::getItemName(){
    return this->weaponName;
}
double ItemWeapon::getItemStat(int stat){
    return this->weaponStats[stat];
}
vector<double> ItemWeapon::getItemStats(){
    return this->weaponStats;
}
double ItemWeapon::getItemSpecial(int special){
    return this->weaponSpecials[special];
}
vector<double> ItemWeapon::getItemSpecials(){
    return this->weaponSpecials;
}
int ItemWeapon::getItemRarity(){
    return this->weaponRarity;
}

void ItemWeapon::setWeaponStats(vector<double> weaponStats, string name){
    this->weaponName = name;
    this->weaponStats = weaponStats;
}

void ItemWeapon::setWeaponSpecials(vector<double> weaponSpecials){
    this->weaponSpecials = weaponSpecials;
}

void ItemWeapon::setWeaponParts(int rarity, int type, int prefix, int suffix){
    this->weaponRarity = rarity;
    this->weaponType = type;
    this->weaponPrefix = prefix;
    this->weaponSuffix = suffix;
}

void ItemWeapon::update(){
    if(this->entityWorldPosition != worldPosition && this->usesWorldPosition){
        return;
    }
    this->hoveringOver = false;
    if(checkCollision(this->posX, this->posY, mouseX, mouseY, this->width, this->height, 0, 0)){
        this->hoveringOver = true;
        if(mouseButtonLeftClick){
            playerList[0]->equipWeapon(this->entityId);
        }
    }
}

void ItemWeapon::draw(){
    if(this->entityWorldPosition != worldPosition && this->usesWorldPosition){
        return;
    }

    al_draw_filled_rectangle(this->posX, this->posY, this->posX+this->width, this->posY+this->height, lootSystem.getRarityColor(this->weaponRarity));
    if(this->hoveringOver){
        this->drawInformationBox();
    }
}

void ItemWeapon::drawInformationBox(){
    ALLEGRO_COLOR rarityColor = lootSystem.getRarityColor(this->weaponRarity);

    double boxWidth = 450, boxHeight = 250;

    al_draw_filled_rectangle(mouseX, mouseY-boxHeight, mouseX+boxWidth, mouseY, al_map_rgb(20, 20, 20));
    al_draw_text(defaultFont, rarityColor, mouseX, mouseY-boxHeight, 0, this->weaponName.c_str());
    al_draw_textf(smallFont, al_map_rgb(235, 190, 65), mouseX, mouseY-boxHeight+23, 0,  "Item Level: %.2f", this->itemLevel);
    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-boxHeight+40, 0, "Damage: %.2f-%.2f", this->weaponStats[weaponMinDamage], this->weaponStats[weaponMaxDamage]);
    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-boxHeight+55, 0, "Fire Rate: %.2f shots/s", 1/this->weaponStats[weaponFireRate]);
    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-boxHeight+70, 0, "Shot Speed: %.2f tiles/s", this->weaponStats[weaponShotSpeed]/tileSize);
    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-boxHeight+85, 0, "Accuracy: %.2f deg spread", 25.5-this->weaponStats[weaponAccuracy]*25.5);
    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-boxHeight+100, 0, "Crit Chance: %.2f%%", this->weaponStats[weaponCritChance]*100);
    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-boxHeight+115, 0, "Crit Strength: %.2f%%", this->weaponStats[weaponCritStrength]*100);
    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-boxHeight+130, 0, "Armor Penetration: %.2f%%", this->weaponStats[weaponArmorPenetration]*100);

    int specialsDrawn = 0;
    for(int i = 0; i < this->weaponSpecials.size(); i++){
        if(this->weaponSpecials[i] != 0){
            switch(i){
                case weaponFireStrength:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Fire Strength: %.2f", this->weaponSpecials[weaponFireStrength]); break;
                case weaponPoisionStrength:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Poison Strength: %.2f", this->weaponSpecials[weaponPoisionStrength]); break;
                case weaponElectricStrength:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Electric Strength: %.2f", this->weaponSpecials[weaponElectricStrength]); break;
                case weaponSlowStrenght:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Slow Strength: %.2f%%", this->weaponSpecials[weaponSlowStrenght]*100); break;
                case weaponAdditionalBullets:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Additional Bullets: %.2f", this->weaponSpecials[weaponAdditionalBullets]); break;
                case weaponExplosionRadius:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Explosion Radius: %.2f tiles", this->weaponSpecials[weaponExplosionRadius]/tileSize); break;
                case weaponBulletSplit:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Splits Into Bullets: %.2f", this->weaponSpecials[weaponBulletSplit]); break;
                case weaponRicochetAmount:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Ricochet Amount: %.2f", this->weaponSpecials[weaponRicochetAmount]); break;
                case weaponHomingForce:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Homing Force: %.2f%%", this->weaponSpecials[weaponHomingForce]*100); break;
                case weaponVampirism:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Vampirism: %.2f%%", this->weaponSpecials[weaponVampirism]*100); break;
                case weaponExecutionTreshold:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Execution Treshold: %.2f%%", this->weaponSpecials[weaponExecutionTreshold]*100); break;
                case weaponPenetrationAmount:
                    al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(90-specialsDrawn*15), 0, "Penetration Amount: %.2f", this->weaponSpecials[weaponPenetrationAmount]); break;
            }
            specialsDrawn++;
        }
    }

    if(compareWeapons){
        double smallBoxWidth = 250, smallBoxHeight = 180;

        double offsetX = boxWidth+15;

        int pW = playerList[0]->getPlayerEquippedWeapon();

        ALLEGRO_COLOR equippedRarityColor = lootSystem.getRarityColor(itemList[pW]->getItemRarity());

        al_draw_filled_rectangle(mouseX+offsetX, mouseY-smallBoxHeight, mouseX+offsetX+smallBoxWidth, mouseY, al_map_rgb(10, 10, 10));
        if(playerList[0]->getPlayerHasWeaponEquipped()){
            al_draw_text(mediumFont, equippedRarityColor, mouseX+offsetX, mouseY-smallBoxHeight, 0, itemList[pW]->getItemName().c_str());
            al_draw_textf(tinyFont, al_map_rgb(235, 190, 65), mouseX+offsetX, mouseY-smallBoxHeight+23, 0, "Item Level: %.2f", itemList[pW]->getItemLevel());
            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-smallBoxHeight+40, 0, "Damage: %.2f-%.2f", itemList[pW]->getItemStat(weaponMinDamage), itemList[pW]->getItemStat(weaponMaxDamage));
            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-smallBoxHeight+50, 0, "Fire Rate: %.2f shots/s", 1/itemList[pW]->getItemStat(weaponFireRate));
            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-smallBoxHeight+60, 0, "Shot Speed: %.2f tiles/s", itemList[pW]->getItemStat(weaponShotSpeed)/tileSize);
            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-smallBoxHeight+70, 0, "Accuracy: %.2f deg spread", 25.5-itemList[pW]->getItemStat(weaponAccuracy)*25.5);
            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-smallBoxHeight+80, 0, "Crit Chance: %.2f%%", itemList[pW]->getItemStat(weaponCritChance)*100);
            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-smallBoxHeight+90, 0, "Crit Strength: %.2f%%", itemList[pW]->getItemStat(weaponCritStrength)*100);
            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-smallBoxHeight+100, 0, "Armor Penetration: %.2f%%", itemList[pW]->getItemStat(weaponArmorPenetration)*100);

            al_draw_textf(smallFont, rarityColor, mouseX+boxWidth, mouseY-boxHeight+40, ALLEGRO_ALIGN_RIGHT, "Min: %+.2f Max: %+.2f", this->weaponStats[weaponMinDamage]-itemList[pW]->getItemStat(weaponMinDamage), this->weaponStats[weaponMaxDamage]-itemList[pW]->getItemStat(weaponMaxDamage));
            al_draw_textf(smallFont, rarityColor, mouseX+boxWidth, mouseY-boxHeight+55, ALLEGRO_ALIGN_RIGHT, "%+.2f shots/s", 1/this->weaponStats[weaponFireRate]-1/itemList[pW]->getItemStat(weaponFireRate));
            al_draw_textf(smallFont, rarityColor, mouseX+boxWidth, mouseY-boxHeight+70, ALLEGRO_ALIGN_RIGHT, "%+.2f tiles/s", (this->weaponStats[weaponShotSpeed]-itemList[pW]->getItemStat(weaponShotSpeed))/tileSize);
            al_draw_textf(smallFont, rarityColor, mouseX+boxWidth, mouseY-boxHeight+85, ALLEGRO_ALIGN_RIGHT, "%+.2f deg", -(this->weaponStats[weaponAccuracy]-itemList[pW]->getItemStat(weaponAccuracy))*25.5);
            al_draw_textf(smallFont, rarityColor, mouseX+boxWidth, mouseY-boxHeight+100, ALLEGRO_ALIGN_RIGHT, "%+.2f%%", (this->weaponStats[weaponCritChance]-itemList[pW]->getItemStat(weaponCritChance))*100);
            al_draw_textf(smallFont, rarityColor, mouseX+boxWidth, mouseY-boxHeight+115, ALLEGRO_ALIGN_RIGHT, "%+.2f%%", (this->weaponStats[weaponCritStrength]-itemList[pW]->getItemStat(weaponCritStrength))*100);
            al_draw_textf(smallFont, rarityColor, mouseX+boxWidth, mouseY-boxHeight+130, ALLEGRO_ALIGN_RIGHT, "%+.2f%%", (this->weaponStats[weaponArmorPenetration]-itemList[pW]->getItemStat(weaponArmorPenetration))*100);

            int specialsDrawn = 0;
            for(int i = 0; i < this->weaponSpecials.size(); i++){
                if(itemList[pW]->getItemSpecial(i) != 0){
                    switch(i){
                        case weaponFireStrength:
                            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*10), 0, "Fire Strength: %.2f", itemList[pW]->getItemSpecial(weaponFireStrength)); break;
                        case weaponPoisionStrength:
                            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*10), 0, "Poison Strength: %.2f", itemList[pW]->getItemSpecial(weaponPoisionStrength)); break;
                        case weaponElectricStrength:
                            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*10), 0, "Electric Strength: %.2f", itemList[pW]->getItemSpecial(weaponElectricStrength)); break;
                        case weaponSlowStrenght:
                            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*10), 0, "Slow Strength: %.2f%%", itemList[pW]->getItemSpecial(weaponSlowStrenght)*100); break;
                        case weaponAdditionalBullets:
                            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*10), 0, "Additional Bullets: %.2f", itemList[pW]->getItemSpecial(weaponAdditionalBullets)); break;
                        case weaponExplosionRadius:
                            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*10), 0, "Explosion Radius: %.2f tiles", itemList[pW]->getItemSpecial(weaponExplosionRadius)/tileSize); break;
                        case weaponBulletSplit:
                            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*10), 0, "Splits Into Bullets: %.2f", itemList[pW]->getItemSpecial(weaponBulletSplit)); break;
                        case weaponRicochetAmount:
                            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*10), 0, "Ricochet Amount: %.2f", itemList[pW]->getItemSpecial(weaponRicochetAmount)); break;
                        case weaponHomingForce:
                            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*10), 0, "Homing Force: %.2f%%", itemList[pW]->getItemSpecial(weaponHomingForce)*100); break;
                        case weaponVampirism:
                            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*10), 0, "Vampirism: %.2f%%", itemList[pW]->getItemSpecial(weaponVampirism)*100); break;
                        case weaponExecutionTreshold:
                            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*10), 0, "Execution Treshold: %.2f%%", itemList[pW]->getItemSpecial(weaponExecutionTreshold)*100); break;
                        case weaponPenetrationAmount:
                            al_draw_textf(tinyFont, equippedRarityColor, mouseX+offsetX, mouseY-(60-specialsDrawn*10), 0, "Penetration Amount: %.2f", itemList[pW]->getItemSpecial(weaponPenetrationAmount)); break;
                    }
                    specialsDrawn++;
                }
            }
        }else{
            al_draw_text(mediumFont, equippedRarityColor, mouseX+offsetX, mouseY-smallBoxHeight, 0, "Player has no weapon equipped");
        }
    }
}
