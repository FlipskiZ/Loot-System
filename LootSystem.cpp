#include "LootSystem.h"
#include "Engine.h"

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

struct ARMOR_STATS{
    string name;
    double armorPiece;
    double armorValue;
    double encumbranceRating;
    double additionalSpecial;
};

//Name - Min Damage - Max Damage - Fire Rate - Shot Speed - Accuracy - Crit Chance - Crit Strength - Armor Penetration - Additional Special (ex. shotgun has multiple bullets, laser has burning etc.)

vector<WEAPON_STATS> s_gunType = { //Constant Numbers. The further in the array they are, the better/more unique they should be.
    {"Pistol", 1, 1, 0.5, 256, 0.95, 0, 1, 0.35, weaponSpecialNone},
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

vector<WEAPON_STATS> s_gunPrefix = { //Percentage Modification. 25% to get none/normal.
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

vector<WEAPON_STATS> s_gunSuffix = { //Percentage Modification. 25% to get none/normal.
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

//Name - Armor Type - Armor Value - Encumberance Rating - Additional Special (ex. boots have movement speed etc.)

vector<ARMOR_STATS> s_armorType = { //Constant Numbers. The further in the array they are, the better/more unique they should be. But armor comes in 'sets'
    {"Wood Helmet", armorHelmet, 2, 1.5, armorSpecialNone},
    {"Wood Chest Armor", armorChestArmor, 5, 3, armorSpecialNone},
    {"Wood Gloves", armorGloves, 1, 0.5, armorSpecialNone},
    {"Wood Pants", armorPants, 3, 2, armorSpecialNone},
    {"Wood Boots", armorBoots, 1, 0.5, armorAdditionalMovementSpeed},
    {"Straw Cape", armorCape, 1, 1, armorSpecialNone},

    {"Copper Helmet", armorHelmet, 4, 3, armorSpecialNone},
    {"Copper Chest Armor", armorChestArmor, 8, 6, armorSpecialNone},
    {"Copper Gloves", armorGloves, 2, 2, armorSpecialNone},
    {"Copper Pants", armorPants, 6, 4, armorSpecialNone},
    {"Copper Boots", armorBoots, 2, 1, armorAdditionalMovementSpeed},
    {"Cloth Cape", armorCape, 1, 0.25, armorSpecialNone},
};

//Name - Armor Value - Encumberance Rating

vector<ARMOR_STATS> s_armorPrefix = { //Percentage Modification. 25% to get none/normal.
    {"Normal", armorPieceNone, 1, 1}, //Reason for the armorPieceNone is so that the other values won't be offset by one back
    {"Good", armorPieceNone, 1.5, 1},
    {"Heavy", armorPieceNone, 2, 1.5},
    {"Bad", armorPieceNone, 0.75, 1},
    {"Lightweight", armorPieceNone, 0.75, 1},
};

//Name - Armor Value - Encumberance Rating

vector<ARMOR_STATS> s_armorSuffix = { //Percentage Modification. 25% to get none/normal.
    {"Of Normalness", armorPieceNone, 1, 1}, //Reason for the armorPieceNone is so that the other values won't be offset by one back
    {"Of Goodness", armorPieceNone, 1.5, 1},
    {"Of Defense", armorPieceNone, 2, 1.5},
    {"Of Badness", armorPieceNone, 0.75, 1},
    {"Of Lightness", armorPieceNone, 0.75, 1},
};

LootSystem::LootSystem(){
    this->weaponStats.resize(AMOUNT_WEAPON_STATS);
    this->weaponSpecials.resize(AMOUNT_WEAPON_SPECIALS);
    this->armorStats.resize(AMOUNT_ARMOR_STATS);
    this->armorSpecials.resize(AMOUNT_ARMOR_SPECIALS);
}

void LootSystem::init(){

}

double LootSystem::getGunType(int type, int element){
    switch(element){
        case 0:
            return s_gunType[type].minDamage; break;
        case 1:
            return s_gunType[type].maxDamage; break;
        case 2:
            return s_gunType[type].fireRate; break;
        case 3:
            return s_gunType[type].shotSpeed; break;
        case 4:
            return s_gunType[type].accuracy; break;
        case 5:
            return s_gunType[type].critChance; break;
        case 6:
            return s_gunType[type].critStrength; break;
        case 7:
            return s_gunType[type].armorPenetration; break;
    }
    return 0;
}

int LootSystem::getGunTypeAdditionalSpecial(int type){
    return s_gunType[type].additionalSpecial;
}

double LootSystem::getGunPrefix(int prefix, int element){
    switch(element){
        case 0:
            return s_gunPrefix[prefix].minDamage; break;
        case 1:
            return s_gunPrefix[prefix].maxDamage; break;
        case 2:
            return s_gunPrefix[prefix].fireRate; break;
        case 3:
            return s_gunPrefix[prefix].shotSpeed; break;
        case 4:
            return s_gunPrefix[prefix].accuracy; break;
        case 5:
            return s_gunPrefix[prefix].critChance; break;
        case 6:
            return s_gunPrefix[prefix].critStrength; break;
        case 7:
            return s_gunPrefix[prefix].armorPenetration; break;
    }
    return 0;
}

double LootSystem::getGunSuffix(int suffix, int element){
    switch(element){
        case 0:
            return s_gunSuffix[suffix].minDamage; break;
        case 1:
            return s_gunSuffix[suffix].maxDamage; break;
        case 2:
            return s_gunSuffix[suffix].fireRate; break;
        case 3:
            return s_gunSuffix[suffix].shotSpeed; break;
        case 4:
            return s_gunSuffix[suffix].accuracy; break;
        case 5:
            return s_gunSuffix[suffix].critChance; break;
        case 6:
            return s_gunSuffix[suffix].critStrength; break;
        case 7:
            return s_gunSuffix[suffix].armorPenetration; break;
    }
    return 0;
}

string LootSystem::getGunTypeName(int type){return s_gunType[type].name;}
string LootSystem::getGunPrefixName(int prefix){return s_gunPrefix[prefix].name;}
string LootSystem::getGunSuffixName(int suffix){return s_gunSuffix[suffix].name;}

double LootSystem::getRandWeaponSpecialStrength(int special, double level){
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

int LootSystem::createWeapon(double level){
    this->weaponName = ""; //9 types of standard weapon statistics, 8 without the name
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

    randNum = randDouble(level/(100/0.9), 0.1+level/(100/0.9)); /**Gets a random weapon type within a slice of the weapon types pool. level is divided by (0.9^-1)*100 so that we have a 0.1 gap that the weapon types can be chosen from
    This slice goes up with levels. At level 1, you have access to only the first 10% of weapon types. All the way to level 100 where you have access to the last 10% of weapon types.**/
    double oneSlice = (double)1/s_gunType.size(); //Split up the weapon types evenly through 0 and 1.
    double currentSlice = 0;

    for(int i = 0; i < s_gunType.size(); i++){
        currentSlice += oneSlice;
        if(randNum < currentSlice){
            this->weaponType = i;
            this->weaponName = getGunTypeName(this->weaponType);
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
        randNum = randInt(1, s_gunPrefix.size()-1);
        this->weaponPrefix = randNum;
    }

    this->weaponName.insert(0, " ");
    this->weaponName.insert(0, getGunPrefixName(this->weaponPrefix));

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
        randNum = randInt(1, s_gunSuffix.size()-1);
        this->weaponSuffix = randNum;
    }

    this->weaponName += " ";
    this->weaponName += getGunSuffixName(weaponSuffix);

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
        this->weaponSpecials[additionalSpecial] = getRandWeaponSpecialStrength(additionalSpecial, level);
    }

    for(int i = 0; i < this->weaponRarity; i++){
        do{
            randNum = randInt(0, AMOUNT_WEAPON_SPECIALS-1);
        }while(this->weaponSpecials[randNum] != 0);
        this->weaponSpecials[randNum] = getRandWeaponSpecialStrength(randNum, level);
    }

    //Automatically add this to the player inventory. Temporary
    int width = 64, height = 64;
    unique_ptr<ItemWeapon> newWeapon(new ItemWeapon());
    newWeapon->setPos(0, 0);
    newWeapon->setDimensions(width, height);
    newWeapon->setWeaponStats(this->weaponStats, this->weaponName);
    newWeapon->setWeaponSpecials(this->weaponSpecials);
    newWeapon->setWeaponParts(this->weaponRarity, this->weaponType, this->weaponPrefix, this->weaponSuffix);
    newWeapon->setItemLevel(level);
    int id = addWeaponToList(move(newWeapon));
    /*if(playerList[0]->getInventoryUsed() < playerList[0]->getMaxInventorySpace()){
        playerList[0]->addItemToInventory(id);
    }*/
    return id;
}

double LootSystem::getArmorType(int type, int element){
    switch(element){
        case 0:
            return s_armorType[type].armorValue; break;
        case 1:
            return s_armorType[type].encumbranceRating; break;
    }
    return 0;
}

int LootSystem::getArmorTypeAdditionalSpecial(int type){
    return s_armorType[type].additionalSpecial;
}

int LootSystem::getArmorTypeArmorPiece(int type){
    return s_armorType[type].armorPiece;
}

double LootSystem::getArmorPrefix(int prefix, int element){
    switch(element){
        case 0:
            return s_armorPrefix[prefix].armorValue; break;
        case 1:
            return s_armorPrefix[prefix].encumbranceRating; break;
    }
    return 0;
}

double LootSystem::getArmorSuffix(int suffix, int element){
    switch(element){
        case 0:
            return s_armorSuffix[suffix].armorValue; break;
        case 1:
            return s_armorSuffix[suffix].encumbranceRating; break;
    }
    return 0;
}

string LootSystem::getArmorTypeName(int type){return s_armorType[type].name;}
string LootSystem::getArmorPrefixName(int prefix){return s_armorPrefix[prefix].name;}
string LootSystem::getArmorSuffixName(int suffix){return s_armorSuffix[suffix].name;}

double LootSystem::getRandArmorSpecialStrength(int special, double level){
    //Get a random strength depending on the type of special, level and weapon damage
    switch(special){
        case armorAdditionalMovementSpeed:
            return randDouble(1+level/(100.0/16), 16+level/(100.0/16)); break; //From 1 to 32 - 16 variable by level
        case armorDamageReflection:
            return randDouble(level/(100/0.5), 0.5+level/(100/0.5)); break; //From 0 to 1 - 0.5 variable by level
        case armorAdditionalLife:
            return randDouble(level/(100/20), 20+level/(100/80)); break; //From 0 to 100 - 80 Variable by level
        case armorRetributionDamage:
            return randDouble(level/(100.0/0.3), 0.5+level/(100.0/0.5)); break; //From 0 to 1 - 0.5 variable by level
        case armorSlowAuraStrength:
            return randDouble(level/(100/0.5), 0.5+level/(100/0.5)); break; //from 0 to 1 - 0.5 variable
        case armorFireAuraStrength:
            return randDouble(level/(100/50), level/(100/100)); break; //from 0 to 100 - 100 variable
    }
    return 0;
}

int LootSystem::createArmor(double level){
    this->armorName = ""; //9 types of standard weapon statistics, 8 without the name
    fill(armorStats.begin(), armorStats.end(), 0);/*
    this->minDamage = 0;
    this->maxDamage = 0;
    this->fireRate = 0;
    this->shotSpeed = 0;
    this->accuracy = 0;
    this->critChance = 0;
    this->critStrength = 0;
    this->armorPenetration = 0;*/

    fill(armorSpecials.begin(), armorSpecials.end(), 0);/*
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
        armorRarity = itemCommon;
    }else if(randNum < 0.70){
        armorRarity = itemUncommon;
    }else if(randNum < 0.85){
        armorRarity = itemRare;
    }else if(randNum < 0.93){
        armorRarity = itemEpic;
    }else if(randNum < 0.98){
        armorRarity = itemLegendary;
    }else{
        armorRarity = itemUnique;
    }

    randNum = randDouble(level/(100/0.8), 0.2+level/(100/0.8)); /**Gets a random armor set type within a slice of the armor set types pool. level is divided by (0.8^-1)*100 so that we have a 0.2 gap that the armor set types can be chosen from
    This slice goes up with levels. At level 1, you have access to only the first 20% of armor set types. All the way to level 100 where you have access to the last 20% of armor set types.**/
    double oneSlice = (double)1/(s_armorType.size()/6); //Split up the armor set types evenly through 0 and 1. one set is 6 pieces
    double currentSlice = 0;

    double withinSet = randInt(0, 5); //0-5 of the armor types. Which totals up to 6 pieces

    for(int i = 0; i < s_armorType.size()/6; i++){
        currentSlice += oneSlice;
        if(randNum < currentSlice){
            this->armorType = i+withinSet;
            this->armorName = getArmorTypeName(this->armorType);
            for(int j = 0; j < AMOUNT_ARMOR_STATS; j++){
                this->armorStats[j] = getArmorType(this->armorType, j);
            }
            break;
        }
    }

    //this->armorStats[armorValue] *= level; //Possibly scale with levels

    if(randInt(0, 3) == 0){
        this->armorPrefix = 0; //25% chance for it to be normal/none.
    }else{
        randNum = randInt(1, s_armorPrefix.size()-1);
        this->armorPrefix = randNum;
    }

    this->armorName.insert(0, " ");
    this->armorName.insert(0, getArmorPrefixName(this->armorPrefix));

    for(int i = 0; i < AMOUNT_ARMOR_STATS; i++){
        this->armorStats[i] *= getArmorPrefix(this->armorPrefix, i);
    }
    if(randInt(0, 3) == 0){
        this->armorSuffix = 0; //25% chance for it to be normal/none.
    }else{
        randNum = randInt(1, s_armorSuffix.size()-1);
        this->armorSuffix = randNum;
    }

    this->armorName += " ";
    this->armorName += getArmorSuffixName(armorSuffix);

    for(int i = 0; i < AMOUNT_ARMOR_STATS; i++){
        this->armorStats[i] *= getArmorSuffix(this->armorSuffix, i);
    }

    int additionalSpecial = this->getArmorTypeAdditionalSpecial(this->armorType);
    if(additionalSpecial != armorSpecialNone){
        this->armorSpecials[additionalSpecial] = getRandArmorSpecialStrength(additionalSpecial, level);
    }

    this->armorPiece = this->getArmorTypeArmorPiece(this->armorType);

    for(int i = 0; i < this->armorRarity; i++){
        do{
            randNum = randInt(0, AMOUNT_ARMOR_SPECIALS-1);
        }while(this->armorSpecials[randNum] != 0);
        this->armorSpecials[randNum] = getRandArmorSpecialStrength(randNum, level);
    }

    int width = 64, height = 64;
    unique_ptr<ItemArmor> newArmor(new ItemArmor());
    newArmor->setPos(0, 0);
    newArmor->setDimensions(width, height);
    newArmor->setArmorStats(this->armorStats, this->armorName, this->armorPiece);
    newArmor->setArmorSpecials(this->armorSpecials);
    newArmor->setArmorParts(this->armorRarity, this->armorType, this->armorPrefix, this->armorSuffix);
    newArmor->setItemLevel(level);
    int id = addArmorToList(move(newArmor));
    /*if(playerList[0]->getInventoryUsed() < playerList[0]->getMaxInventorySpace()){
        playerList[0]->addItemToInventory(id);
    }*/
    return id;
}
