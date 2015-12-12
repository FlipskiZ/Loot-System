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
    this->hoveringOver = false;
    if(checkCollision(this->posX, this->posY, mouseX, mouseY, this->width, this->height, 0, 0)){
        this->hoveringOver = true;
        if(mouseButtonLeftClick){
            playerList[0]->equipWeapon(this->entityId);
        }
    }
}

void ItemWeapon::draw(){
    ALLEGRO_COLOR rarityColor = lootSystem.getRarityColor(this->weaponRarity);
    al_draw_filled_rectangle(this->posX, this->posY, this->posX+this->width, this->posY+this->height, rarityColor);
    if(this->hoveringOver){
        al_draw_filled_rectangle(mouseX, mouseY-220, mouseX+500, mouseY, al_map_rgb(0, 0, 0));
        al_draw_text(defaultFont, rarityColor, mouseX, mouseY-220, 0, this->weaponName.c_str());
        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-200, 0, "Damage: %.2f-%.2f", this->weaponStats[weaponMinDamage], this->weaponStats[weaponMaxDamage]);
        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-185, 0, "Fire Rate: %.2f shots/s", 1/this->weaponStats[weaponFireRate]);
        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-170, 0, "Shot Speed: %.2f tiles/s", this->weaponStats[weaponShotSpeed]/tileSize);
        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-155, 0, "Accuracy: %.2f deg spread", 25.5-this->weaponStats[weaponAccuracy]*25.5);
        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-140, 0, "Crit Chance: %.2f%%", this->weaponStats[weaponCritChance]*100);
        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-125, 0, "Crit Strength: %.2f%%", this->weaponStats[weaponCritStrength]*100);
        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-110, 0, "Armor Penetration: %.2f%%", this->weaponStats[weaponArmorPenetration]*100);

        al_draw_textf(smallFont, rarityColor, mouseX+500, mouseY-200, ALLEGRO_ALIGN_RIGHT, "Min: %+.2f%% Max: %+.2f%%", (lootSystem.getGunPrefix(this->weaponPrefix, weaponMinDamage)*lootSystem.getGunSuffix(this->weaponSuffix, weaponMinDamage)-1)*100, (lootSystem.getGunPrefix(this->weaponPrefix, weaponMaxDamage)*lootSystem.getGunSuffix(this->weaponSuffix, weaponMaxDamage)-1)*100);
        al_draw_textf(smallFont, rarityColor, mouseX+500, mouseY-185, ALLEGRO_ALIGN_RIGHT, "%+.2f%%", (lootSystem.getGunPrefix(this->weaponPrefix, weaponFireRate)*lootSystem.getGunSuffix(this->weaponSuffix, weaponFireRate)-1)*100);
        al_draw_textf(smallFont, rarityColor, mouseX+500, mouseY-170, ALLEGRO_ALIGN_RIGHT, "%+.2f%%", (lootSystem.getGunPrefix(this->weaponPrefix, weaponShotSpeed)*lootSystem.getGunSuffix(this->weaponSuffix, weaponShotSpeed)-1)*100);
        al_draw_textf(smallFont, rarityColor, mouseX+500, mouseY-155, ALLEGRO_ALIGN_RIGHT, "%+.2f%%", (lootSystem.getGunPrefix(this->weaponPrefix, weaponAccuracy)*lootSystem.getGunSuffix(this->weaponSuffix, weaponAccuracy)-1)*100);
        al_draw_textf(smallFont, rarityColor, mouseX+500, mouseY-140, ALLEGRO_ALIGN_RIGHT, "%+.2f%%", (lootSystem.getGunPrefix(this->weaponPrefix, weaponCritChance)*lootSystem.getGunSuffix(this->weaponSuffix, weaponCritChance)-1)*100);
        al_draw_textf(smallFont, rarityColor, mouseX+500, mouseY-125, ALLEGRO_ALIGN_RIGHT, "%+.2f%%", (lootSystem.getGunPrefix(this->weaponPrefix, weaponCritStrength)*lootSystem.getGunSuffix(this->weaponSuffix, weaponCritStrength)-1)*100);
        al_draw_textf(smallFont, rarityColor, mouseX+500, mouseY-110, ALLEGRO_ALIGN_RIGHT, "%+.2f%%", (lootSystem.getGunPrefix(this->weaponPrefix, weaponArmorPenetration)*lootSystem.getGunSuffix(this->weaponSuffix, weaponArmorPenetration)-1)*100);

        int specialsDrawn = 0;
        for(int i = 0; i < this->weaponSpecials.size(); i++){
            if(this->weaponSpecials[i] != 0){
                switch(i){
                    case weaponFireStrength:
                        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(80-specialsDrawn*15), 0, "Fire Strength: %.2f", this->weaponSpecials[weaponFireStrength]); break;
                    case weaponPoisionStrength:
                        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(80-specialsDrawn*15), 0, "Poison Strength: %.2f", this->weaponSpecials[weaponPoisionStrength]); break;
                    case weaponElectricStrength:
                        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(80-specialsDrawn*15), 0, "Electric Strength: %.2f", this->weaponSpecials[weaponElectricStrength]); break;
                    case weaponSlowStrenght:
                        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(80-specialsDrawn*15), 0, "Slow Strength: %.2f%%", this->weaponSpecials[weaponSlowStrenght]*100); break;
                    case weaponAdditionalBullets:
                        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(80-specialsDrawn*15), 0, "Additional Bullets: %.2f", this->weaponSpecials[weaponAdditionalBullets]); break;
                    case weaponExplosionRadius:
                        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(80-specialsDrawn*15), 0, "Explosion Radius: %.2f tiles", this->weaponSpecials[weaponExplosionRadius]/tileSize); break;
                    case weaponBulletSplit:
                        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(80-specialsDrawn*15), 0, "Splits Into Bullets: %.2f", this->weaponSpecials[weaponBulletSplit]); break;
                    case weaponRicochetAmount:
                        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(80-specialsDrawn*15), 0, "Ricochet Amount: %.2f", this->weaponSpecials[weaponRicochetAmount]); break;
                    case weaponHomingForce:
                        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(80-specialsDrawn*15), 0, "Homing Force: %.2f%%", this->weaponSpecials[weaponHomingForce]*100); break;
                    case weaponVampirism:
                        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(80-specialsDrawn*15), 0, "Vampirism: %.2f%%", this->weaponSpecials[weaponVampirism]*100); break;
                    case weaponExecutionTreshold:
                        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(80-specialsDrawn*15), 0, "Execution Treshold: %.2f%%", this->weaponSpecials[weaponExecutionTreshold]*100); break;
                    case weaponPenetrationAmount:
                        al_draw_textf(smallFont, rarityColor, mouseX, mouseY-(80-specialsDrawn*15), 0, "Penetration Amount: %.2f", this->weaponSpecials[weaponPenetrationAmount]); break;
                }
                specialsDrawn++;
            }
        }
    }
}
