#include "LootSystem.h"
#include "Engine.h"

struct WEAPON_STATS{
    string name;
    double minDamage;
    double maxDamage;
    double fireRate;
    double shotSpeed;
    double accuracy;
    double critChance;
    double critStrength;
    double armorPenetration;
    double additionalSpecial;
};

//Name - Min Damage - Max Damage - Fire Rate - Shot Speed - Accuracy - Crit Chance - Crit Strength - Armor Penetration - Additional Special (ex. shotgun has multiple bullets, laser has burning etc.)

vector<WEAPON_STATS> gunType = { //Constant Numbers. The further in the array they are, the better/more unique they should be.
    {"Pistol", 1, 1, 0.5, 256, 0.95, 0, 1, 0, weaponSpecialNone},
    {"Rifle", 2, 3, 1, 1024, 1, 0.1, 1.5, 0.2, weaponSpecialNone},
    {"Shotgun", 2, 3, 1, 512, 0, 0.15, 1.75, 0.25, weaponAdditionalBullets},
    {"Machine Gun", 1, 2, 0.0625, 512, 0.9, 0.05, 1.25, 0.1, weaponSpecialNone},
    {"Railgun", 4, 5, 1.5, 2048, 2, 0.2, 2, 0.3, weaponPenetrationAmount},
    {"RPG", 3, 4, 1, 256, 0.8, 0.1, 1.5, 0.25, weaponExplosionRadius},
    {"Plasma Pistol", 2, 4, 0.35, 512, 0.8, 0.2, 2.3, 0.5, weaponFireStrength},
    {"Plasma Machine Gun", 2, 3, 0.1, 768, 0.8, 0.25, 2.5, 0.4, weaponFireStrength},
    {"Homing Gun", 1, 2, 0, 512, 0.8, 0.25, 2.5, 0.4, weaponHomingForce},
};

//Name - Min Damage - Max Damage - Fire Rate - Shot Speed - Accuracy - Crit Chance - Crit Strength - Armor Penetration

vector<WEAPON_STATS> gunPrefix = { //Percentage Modification. 25% to get none/normal.
    {"Normal", 1, 1, 1, 1, 1, 1, 1, 1},
    {"Good", 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5},
    {"Sawed-Off", 2, 3, 1, 1, 0.75, 1.05, 1.25, 0.9},
    {"Bad", 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75},
    {"Strong", 1.75, 2, 1, 1, 1, 1.2, 1.2, 1.1},
    {"Angry", 2, 3, 2, 1.25, 0.5, 0.75, 1, 0},
    {"Old", 0.6, 0.6, 0.75, 0.8, 0.9, 0.75, 0.75, 0.75},
    {"Fast", 0.8, 0.9, 1.35, 2.5, 1, 1, 1, 0.9},
    {"Intense", 1.2, 1.35, 0.9, 1.15, 0.9, 1.1, 1.2, 1.15},
    {"Feeble", 0.6, 0.75, 1, 1, 0.9, 0.85, 0.9, 0.9},
    {"Frail", 0.4, 0.5, 1, 1, 0.85, 0.85, 0.85, 0.85},
};

//Name - Min Damage - Max Damage - Fire Rate - Shot Speed - Accuracy - Crit Chance - Crit Strength - Armor Penetration

vector<WEAPON_STATS> gunSuffix = { //Percentage Modification. 25% to get none/normal.
    {"Of Normalness", 1, 1, 1, 1, 1, 1, 1, 1},
    {"Of Goodness", 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5},
    {"Of Sniping", 2, 2, 0.5, 1.5, 1.25, 1.25, 1, 1},
    {"Of Badness", 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75},
    {"Of Strength", 1.75, 2, 1, 1, 1, 1.2, 1.2, 1.1},
    {"Of Heavyness", 2, 2, 0.75, 0.75, 0.75, 1.25, 1.25, 0.75},
    {"Of Laziness", 0.7, 0.7, 0.7, 0.5, 0.8, 0.75, 0.75, 0.75},
    {"Of Swiftness", 0.8, 0.9, 1.35, 2.5, 1, 1, 1, 0.9},
    {"Of Powefullness", 1.2, 1.35, 1, 1, 1, 1, 1, 1},
    {"Of Feebleness", 0.8, 0.9, 0.95, 0.95, 0.95, 0.95, 0.95, 0.9},
    {"Of The Weak", 0.6, 0.75, 1, 1, 1, 1, 1, 1},
};

LootSystem::LootSystem(){
    this->weaponSpecials.resize(AMOUNT_WEAPON_SPECIALS);
    this->weaponStats.resize(AMOUNT_WEAPON_STATS);
}

void LootSystem::init(){

}

double LootSystem::getGunType(int type, int element){
    switch(element){
        case 0:
            return gunType[type].minDamage; break;
        case 1:
            return gunType[type].maxDamage; break;
        case 2:
            return gunType[type].fireRate; break;
        case 3:
            return gunType[type].shotSpeed; break;
        case 4:
            return gunType[type].accuracy; break;
        case 5:
            return gunType[type].critChance; break;
        case 6:
            return gunType[type].critStrength; break;
        case 7:
            return gunType[type].armorPenetration; break;
    }
    return 0;
}

int LootSystem::getGunTypeAdditionalSpecial(int type){
    return gunType[type].additionalSpecial;
}

double LootSystem::getGunPrefix(int prefix, int element){
    switch(element){
        case 0:
            return gunPrefix[prefix].minDamage; break;
        case 1:
            return gunPrefix[prefix].maxDamage; break;
        case 2:
            return gunPrefix[prefix].fireRate; break;
        case 3:
            return gunPrefix[prefix].shotSpeed; break;
        case 4:
            return gunPrefix[prefix].accuracy; break;
        case 5:
            return gunPrefix[prefix].critChance; break;
        case 6:
            return gunPrefix[prefix].critStrength; break;
        case 7:
            return gunPrefix[prefix].armorPenetration; break;
    }
    return 0;
}

double LootSystem::getGunSuffix(int suffix, int element){
    switch(element){
        case 0:
            return gunSuffix[suffix].minDamage; break;
        case 1:
            return gunSuffix[suffix].maxDamage; break;
        case 2:
            return gunSuffix[suffix].fireRate; break;
        case 3:
            return gunSuffix[suffix].shotSpeed; break;
        case 4:
            return gunSuffix[suffix].accuracy; break;
        case 5:
            return gunSuffix[suffix].critChance; break;
        case 6:
            return gunSuffix[suffix].critStrength; break;
        case 7:
            return gunSuffix[suffix].armorPenetration; break;
    }
    return 0;
}

string LootSystem::getGunTypeName(int type){return gunType[type].name;}
string LootSystem::getGunPrefixName(int prefix){return gunPrefix[prefix].name;}
string LootSystem::getGunSuffixName(int suffix){return gunSuffix[suffix].name;}

double LootSystem::getRandSpecialStrength(int special, double level){
    //Get a random strength depending on the type of special, level and weapon damage
    switch(special){
        case weaponFireStrength:
            return randDouble(this->weaponStats[weaponMinDamage], this->weaponStats[weaponMaxDamage])/3; break;
        case weaponPoisionStrength:
            return randDouble(this->weaponStats[weaponMinDamage], this->weaponStats[weaponMaxDamage])/3; break;
        case weaponElectricStrength:
            return randDouble(this->weaponStats[weaponMinDamage], this->weaponStats[weaponMaxDamage])/5; break; //Electric deals the ful damage instantly, so it gets a bit less strength
        case weaponSlowStrenght:
            return randDouble(level/(100.0/0.5), 0.5+level/(100.0/0.5)); break; //From 0 to 1 - 0.5 variable by level
        case weaponAdditionalBullets:
            return randDouble(1+level/20, 3+level/20); break; //from 1 to 8 - 5 variable
        case weaponExplosionRadius:
            return randDouble(16+level/2, 32+level*2); break; //from 16 to 232 - 200 variable
        case weaponBulletSplit:
            return randDouble(1+level/20, 3+level/20); break; //1 to 8 - 5 variable
        case weaponRicochetAmount:
            return randDouble(1+level/20, 3+level/20); break; //1 to 8 - 5 variable
        case weaponHomingForce:
            return randDouble(0.1+level/(100.0/0.6), 0.4+level/(100.0/0.6)); break; //0.1 to 1 - 0.6 variable
        case weaponVampirism:
            return randDouble(0.01, 0.03); break; //This will already scale with the damage, this is probably one of the strongest specials. So we leave this at a constant range and low percentage.
        case weaponExecutionTreshold:
            return randDouble(0.2, 0.5); break; //Same thing as Vampirism
        case weaponPenetrationAmount:
            return randDouble(1+level/20, 3+level/20); break; //1 to 8 - 5 variable
    }
    return 0;
}

ALLEGRO_COLOR LootSystem::getRarityColor(int rarity){
    switch(rarity){
        case itemCommon:
            return al_map_rgb(255, 255, 255); break;
        case itemUncommon:
            return al_map_rgb(100, 255, 100); break;
        case itemRare:
            return al_map_rgb(50, 150, 255); break;
        case itemEpic:
            return al_map_rgb(225, 75, 225); break;
        case itemLegendary:
            return al_map_rgb(255, 200, 50); break;
        case itemUnique:
            return al_map_rgb(255, 50, 50); break;
    }
    return al_map_rgb(255, 255, 255);
}

int LootSystem::createWeapon(double level){
    this->name = ""; //9 types of standard weapon statistics, 8 without the name
    fill(weaponStats.begin(), weaponStats.end(), 0);/*
    this->minDamage = 0;
    this->maxDamage = 0;
    this->fireRate = 0;
    this->shotSpeed = 0;
    this->accuracy = 0;
    this->critChance = 0;
    this->critStrength = 0;
    this->armorPenetration = 0;*/

    fill(weaponSpecials.begin(), weaponSpecials.end(), 0);/*
    this->weaponRarity = itemCommon; //45% Normal - 25% uncommon - 15% rare - 10% epic - 4% legendary - 1% unique
    this->fireStrength = 0; //12 types of special modifications
    this->poisionStrength = 0;
    this->electricStrength = 0;
    this->slowStrenght = 0;
    this->additionalBullets = 0;
    this->explosionRadius = 0;
    this->bulletSplit = 0;
    this->ricochetAmount = 0;
    this->homingForce = 0;
    this->vampirism = 0;
    this->executionTreshold = 0;
    this->penetrationAmount = 0;*/

    double randNum = randDouble();

    if(randNum < 0.45){ //Approx. 45% Normal - 25% uncommon - 15% rare - 8% epic - 5% legendary - 2% unique
        weaponRarity = itemCommon;
    }else if(randNum < 0.70){
        weaponRarity = itemUncommon;
    }else if(randNum < 0.85){
        weaponRarity = itemRare;
    }else if(randNum < 0.93){
        weaponRarity = itemEpic;
    }else if(randNum < 0.98){
        weaponRarity = itemLegendary;
    }else{
        weaponRarity = itemUnique;
    }

    randNum = randDouble(level/(100/0.9), 0.1+level/(100/0.9)); ///Gets a random weapon type within a slice of the weapon types pool. level is divided by (0.9^-1)*100 so that we have a 0.1 gap that the weapon types can be chosen from\
    This slice goes up with levels. At level 1, you have access to only the first 10% of weapon types. All the way to level 100 where you have access to the last 10% of weapon types.
    double oneSlice = (double)1/gunType.size(); //Split up the weapon types evenly through 0 and 1.
    double currentSlice = 0;

    for(int i = 0; i < gunType.size(); i++){
        currentSlice += oneSlice;
        if(randNum < currentSlice){
            this->weaponType = i;
            this->name = getGunTypeName(this->weaponType);
            for(int j = 0; j < AMOUNT_WEAPON_STATS; j++){
                this->weaponStats[j] = getGunType(this->weaponType, j);
            }
            break;
        }
    }

    this->weaponStats[weaponMinDamage] *= level;
    this->weaponStats[weaponMaxDamage] *= level;

    if(randInt(0, 3) == 0){
        this->weaponPrefix = 0; //25% chance for it to be normal/none.
    }else{
        randNum = randInt(1, gunPrefix.size()-1);
        this->weaponPrefix = randNum;
    }

    this->name.insert(0, " ");
    this->name.insert(0, getGunPrefixName(this->weaponPrefix));

    for(int i = 0; i < AMOUNT_WEAPON_STATS; i++){
        if(i != weaponFireRate){
            this->weaponStats[i] *= getGunPrefix(this->weaponPrefix, i);
        }else{
            this->weaponStats[i] /= getGunPrefix(this->weaponPrefix, i); //The fire rate is supposed to be divided
        }
    }
    if(randInt(0, 3) == 0){
        this->weaponSuffix = 0; //25% chance for it to be normal/none.
    }else{
        randNum = randInt(1, gunSuffix.size()-1);
        this->weaponSuffix = randNum;
    }

    this->name += " ";
    this->name += getGunSuffixName(weaponSuffix);

    for(int i = 0; i < AMOUNT_WEAPON_STATS; i++){
        if(i != weaponFireRate){
            this->weaponStats[i] *= getGunSuffix(this->weaponSuffix, i);
        }else{
            this->weaponStats[i] /= getGunSuffix(this->weaponSuffix, i); //The fire rate is supposed to be divided
        }
    }

    if(this->weaponStats[weaponAccuracy] > 1){ //If accuracy is over 100% set it to 100%. Accuracy can't be below 0. The closer accuracy is to 0, the bigger the 25.5 degrees slice it can shoot in will be.
        this->weaponStats[weaponAccuracy] = 1;
    }

    int additionalSpecial = this->getGunTypeAdditionalSpecial(this->weaponType);
    if(additionalSpecial != weaponSpecialNone){
        this->weaponSpecials[additionalSpecial] = getRandSpecialStrength(additionalSpecial, level);
    }

    for(int i = 0; i < this->weaponRarity; i++){
        do{
            randNum = randInt(0, AMOUNT_WEAPON_SPECIALS-1);
        }while(this->weaponSpecials[randNum] != 0);
        this->weaponSpecials[randNum] = getRandSpecialStrength(randNum, level);
    }

    //Automatically add this to the player inventory. Temporary
    int width = 64, height = 64;
    unique_ptr<ItemWeapon> newWeapon(new ItemWeapon());
    newWeapon->setPos(0, 0);
    newWeapon->setDimensions(width, height);
    newWeapon->setWeaponStats(this->weaponStats, this->name);
    newWeapon->setWeaponSpecials(this->weaponSpecials);
    newWeapon->setWeaponParts(this->weaponRarity, this->weaponType, this->weaponPrefix, this->weaponSuffix);
    newWeapon->setItemLevel(level);
    int id = addWeaponToList(move(newWeapon));
    /*if(playerList[0]->getInventoryUsed() < playerList[0]->getMaxInventorySpace()){
        playerList[0]->addItemToInventory(id);
    }*/
    return id;
}
