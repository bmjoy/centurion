# Imper4m Project - Workflow

## Gameplay

1. The game has a 3D rendering system, but its view will be set on 2.5D, like *Age of Empires*. Camera will be orthographic and above the battle field, and it can't be neither zoomed nor rotated.

2. There will be no limitation, initially, to the maximum number of units that can be recruited, as they are sprites; this assumption, however, may change if needed for framerate issues in the future.

3. Excluding wood, clay (for tiles) and some other materials, units and buildings have two layers, whose colors depends on certain situations:
    * **Metallic material**: only for military units and ships, it's made up of a color which depends from the combat level. Level 1-5 = dark white/gray; Level 6-11 = white/bright gray; Level 12-17 = bronze; Level 18-1000 = gold.
    * **Material for clothes/mantles/decorations**: usable in buildings too, includes all the very visible parts (such as drapes, cloaks or clothes) whose hue depends on the color of the player who controls them.

4. Selection system will be the same as a typical RTS game: you can select a unit or a building with a single mouse click; you will draw a rectangular area instead, if you will hold down the left mouse button, including every unit that is into this area, but not buildings. You can easily create quick units/buildings selection groups holding CTRL and pressing a number (1-9): this will allows you to select a specific building or a group of units just pressing on the concerning number.

5. You can start a game choosing the game mode in the main menu.
    * **Adventure**. You can't win or lose the game following the normal game rules, but you have to rationally use the resources at your disposal to achieve some objectives. Maps have a "story" you have to follow and are ruled by some scripts will change map or units behavior.
    * **Conquest**. The goal here is to choose a civilization and conquest every region in a vaste territory, proceeding only through neighborhood enemies, step by step: if you would like to fight an harder challenge, you can change the difficulty level. Each civilization has a specific bonus, and each time you will defeat an enemy, you will obtain the conquered civilization benefits. (*Rome*: a level 20 hero + 6,000 gold; *Gaul*: 4 level 12 Fand's warriors + heroes and warriors get experience twice as fast; *Iberia*: warriors heal if they have max energy points + heroes and warriors will get experience over time; *Carthage*: 4 war elephants + warriors will get the "Spoils of War" item when they kill an enemy; *Egypt*: troops will survive without food + troops will receive all Charms item whenever approach the townhall; *Britain*: no fog of war + 30 archers will be spawn in the townhall each time they die; *Germany*: every warrior will have "Learning" skill + all townhalls will produce an extra amount of food).
    * **Single player**. You can customize everything, here:
        * Map type, that will be randomly generated (island, continent, islands, ...), or a map created in the editor;
        * Season;
        * Explored / Not explored;
        * Fog of war / No fog;
        * Low / Normal / High population;
        * Low / Normal / High resources; 
        * Players number, color, civilization, IAs and team;
    The goal is to conquest every enemy fortress; if this happens, the conquered player, with he has at least one military unit alive, has 2 minutes to try to reconquest at least one townhall, otherwise he will lose the game.
    * **Multiplayer**. Same as single player, but you can play the game with other humans players.
    
6. A game is played inside a map, whatever the game mode is, namely a realistic landscape in which both nature and the human hand are present: grassy zones with a lot of vegetation, or mountains with snow, will alternate in civilized areas where there will be only buildings and sculptures dedicated to the Gods.

7. By default (and you can deactivate it), during every game seasons will change every two hours and textures will change (also the sun/environment lighting color). In spring/summer, the chance of rain is very low; in autumn the probability is very hight; in winter the probability is the same of autumn, but rain is replaced by snow. You can also decide about the day/night cycle (aesthetic question, but also strategic). 

8. By pressing the SPACEBAR button it is possible to open the minimap, i.e. a reduced scale representation of the game map. It contains the terrain, the buildings (surrounded by a border of the color corresponding to that of the player who owns them), the decorations, represented faithfully, while the units are represented by dots of the color corresponding to that of the player who check it out; however, the minimap shows only the objects that are in the explored areas and, if the fog of war is also active, the units that are not in an area covered by fog.
   
9. During the course of the game some parts of the map appear to be covered by a semi-transparent fog, while others are in the utmost obscurity. This effect is called *Fog of war*. The dark areas are those that have not yet been explored and on which, of course, there is no information. When you pass through these areas with your units, the ground remains in the open and you can see the enemy units and structures that are there.  
    
10. Among the interacting objects we can include units and buildings. While it is possible to recruit units, then essentially create new ones, it is not possible to construct buildings when the game has already been started. Analyzing the structure of these two concepts, starting from the last, we can identify the buildings as the set of all the structures that the player can control and which have the task of storing resources, training soldiers, carrying out technological research, asking favors to the gods or celebrate some powerful rituals and, in some cases, hide and protect units from possible enemy attacks.

11. Each interacting element in game (unit or a building) belongs to a civilization: this will serve, apart from making a graphic distinction, to divide the peoples by characteristics, culture, technologies and strengths and weaknesses.
    
12. The structures are divided into ten basic categories:
    * **Elements of the fortress**. They include, in a generic way (so the names vary according to the civilization they belong to, but the way it works remains that), the Townhall - which is the central building on which the whole settlement depends -, the Barracks - which is the building used for the recruitment of new units provided that you have the necessary resources and equipment -, the Blacksmith - which is the building responsible for the production of armaments necessary to recruit advanced units - the Tavern - that it is the building where resources are exchanged to obtain strategic advantages - the Temple - which is the building where the services of priests and powers of the gods are obtained -, the Arena - which is the building where the heroes are recruited and the characteristics of the units are improved - and the Wall - which includes the walls, in which the *sentinels* are continuously generated but at a decreasing rate, and the gates that serve to protect the fortress from the assaults of the enemies, preventing their transit;
    * **Outposts and forts**. They include all the buildings in which the resources can be obtained or exchanged and where the armies can be sheltered, obtaining immunity from the attacks of enemy troops, or lurking in ambushes;
    * **Ports**. They are the places where vessels are produced for the transport of resources from one place to another and those from war, for the transport of troops and maritime combat;
    * **Villages**. They are the centers used for the production of food. They are composed of houses, which increase the maximum limit of tenant residents, and the center of the village on which the whole settlement depends;
    * **Teutonic camps**. They are special buildings protected by several knights, who attack everything and everyone and prevent the tent from being conquered; however, the process of conquering the structure is not the same as the other settlements and it is, in fact, necessary that all the defenders have been killed before obtaining control, since the building has no loyalty. When this happens, in fact, the structure is able to provide a discreet support to the new owner since, in exchange for some food units, it allows to recruit additional knights defenders under his own control;
    * **Inns**. These ones are peculiar structures that are found exclusively in the Adventure mode with multiple maps, and that allow some selected units, which must have the Group attribute active, to move from one map to another, keeping their statistics unchanged, simulating a journey: to reach an inn located in another map, it is essential that the units have visited it at least once;
    * **Sacrificial altars (Stonehenge)**. They are places where priests, magicians, shamans or druids gather to celebrate, in exchange for a certain value of loyalty, some powerful rituals that will have influence on all the units and structures of any player in the map, including also the one who celebrates the ritual. Control of the altar, however, is up to the player who brings the most spiritual units to the sacrifice. The list of practicable rituals is as follows:
        * Wind of wisdom. Increase by 1 the level of all the existing units, up to a maximum of 20 (loyalty points required: 10);
        * Famine. Reduces, during the next two minutes, the provisions of each fort, fort, village and supply unit (necessary loyalty points: 20);
        * Ritual of vitality. Returns all the energy to the units, every 5 seconds. The effect lasts 30 seconds (necessary loyalty points: 30);
        * Rain of gold. For 2 minutes slowly increases the gold stocks of all structures (loyalty points needed: 40);
        * Spiritual punishment. Reduce, for one minute, the current health of all spiritual units (necessary loyalty points: 50);
        * Bloodthirsty. For one minute all allied warriors deal damage for an amount independent of the enemy's defense value (necessary loyalty points: 60);
        * Teutonic revolt. It provokes a revolt in all the Teutonic camps (necessary loyalty points: 70);
    * **Catapults**. They are special structures whose task is to destroy any building, with large projectiles like fireballs, huge fire spears and so on. Unlike other constructions, they are capable of interacting with the surrounding environment and can be constructed or dismantled by military troops (maximum 10 for catapult), at any point of the map walkable: the greater the number of soldiers inside the catapult, the greater the rate of fire.
    * **Caves**. They are underground passages that join two areas distant from each other. The units and armies use them to move quickly from one place to another without being detected, so that they can take the enemy by surprise. The peculiarity of this structure is that it must be placed in the gaming world always in pairs, a guarantee that both the entry and the exit are always present.
    * **Places with magical objects**. They are more or less special rocky buildings, man-made in favor of the gods, or to ward off evil, in which, at regular intervals, objects appear able to provide extraordinary powers to the holder.
    
13. Every building has this parameters:
    * **Health**
    * **Max health**
    * **Gold**: indicates the amount of gold currently stored in the building. It can be self-produced in "Gold outposts" with more than 2.000 pieces of currency;
    * **Food**: indicates the amount of food currently stored in the building. It can be self-produced in fortresses or villages;
    * **Population**: indicates the number of inhabitants currently residing in that settlement. This value will only appear in villages and townhalls;
    * **Max population**: 
    * **Loyalty**: shows the level of loyalty that that building or settlement has towards the player who owns it. This value starts from 0 up to 100. While it can increase both automatically (so it will start from the minimum until it has reached its maximum, increasing by one), or manually (through the presence of nearby allies, which increase the value of one every two seconds for each unit and can not in any case exceed ten) never decreases, except when enemy units try to conquer it. This last rule does not apply to the altar of sacrifices, in which, in order to increase loyalty, the presence of spiritual units is necessary in the immediate vicinity, while to reduce it one must initiate the related rituals;

14. Units will head for the main structure (e.g. the Townhall) of the enemy settlement when it is selected for capture, and the consequences that result from it, depend fundamentally on the type of structure we take into consideration.
    * **Fortresses/Strongholds**: the player will take control of the townhall and of all the structures connected to it, providing the resources and the technological level acquired by that settlement even if it had not belonged to him until that moment;
    * **Villages**: the player will take control of the center of the village and of all the houses connected to it, providing the food of that settlement even if it had not belonged to him until that moment;
    * **Outposts/Forts/Guardposts**: since they are single elements, they will be distinctly conquered from each other and will provide, to the player who will take control of it, all the resources that they hold within them;
    
15. A building can be attacked and damaged only by throwing units, whether on foot, on horseback or by ships, or by catapults and siege engines: if some units have been sheltered inside, they will suffer damage for each hit that the structure receives, until they are dead. If the building examined is, instead, a house or a village center, this will begin an inexorable loss of the population, in proportion to the damage suffered. When health falls below 10% of maximum health, the building becomes unusable and can return to full operation only after having repaired it through the appropriate command and upon payment of a sum of gold.
    
16. Units represent the element with which the player can exercise his hegemony and his influence throughout the territory. We can divide them into 10 categories:
    * **Infantry units**: belong to this category all soldiers who fight hand to hand and who do not use any means to move or fight (for example a legionary, a Praetorian or a militia);
    * **Throwing units**: belong to this category all soldiers who fight from distance with more or less conventional weapons (bows and arrows, but also slingshots and spears or javelins) and who do not use means to move;
    * **Horsemen**: belong to this category all those soldiers who fight both body to body and with ranged weapons and who use a horse or an animal-drawn cart to move and fight;
    * **Special units**: belong to this category all those soldiers who can be recruited under special conditions and in limited numbers, given their strength and peculiar characteristics (eg Carthaginian war elephants or Iberian guerrillas);
    * **Heroes**: belong to this category some special soldiers, who are riding a horse and who, given their attitudes in combat, are able to use special abilities that provide support in a fight, and to lead a multitude of soldiers in battle;
    * **Boats**: belong to this category all those ships and boats designed to transport units from one coast to another and to fight with fire arrows;
    * **Systems for the transport of resources**: belong to this category all the means by which it is possible to move gold or food from one settlement to another. This movement can take place by land, with mules and wagons, or by sea;
    * **Siege machines**: belong to this category all those contraptions built with the help of soldiers, such as the rams, whose task is limited exclusively to the breakthrough of the gates of the opposing fortresses and damage to the forts;
    * **Sentinels**: it is part of this category a characteristic type of archers, not controllable by the player, whose graphic appearance faithfully replicates that of the respective civilization to which they belong, who patrol the walls of the fortress and who have the task of defending it from enemy attacks;
    * **Others**: belong to this residual category all those units that can not be cataloged in the previous ones, such as animals (deer, crows, eagles, wolves, wild boars, lions, fish, horses and bears) or evanescent spirits;
    
17. Each unit has different characteristics that distinguish it:
    * **Health**
    * **Max health**
    * **Level**: indicates the combat value of the warrior. The higher the damage is, the more damage will be received. The maximum reachable ceiling is set at one thousand. It also influences the color of the metal parts of the armor;
    * **Experience**: indicates the value of experience that the unit possesses. In order to increase it, it is necessary, first of all, not to have reached the maximum level and then to defeat enemy units in combat. The increase is determined in proportion to the difference in level between the attacker and the target;
    * **Max experience**: indicates the maximum value of experience. Once reached, the unit reaches the next level;
    * **Type of attack**: indicates the type of weapon used by the warrior in combat. It can be sharp (swords, axes, hammers, hammers or elephant tusks) or piercing (spears, javelins, arrows, slings or tridents);
    * **Minimum attack**: indicates the minimum damage value that the unit can inflict on the target and below which it can never go down;
    * **Maximum attack**: indicates the maximum amount of damage the unit can deal to the target. This number, however, can increase exponentially based on the level (the increase will not appear in the top bar, but will be exclusive to the formula for the calculation of damages);
    * **Defense against sharp weapons**: indicates the value of damage against sharp weapons that the unit can absorb;
    * **Defense against piercing weapons**: indicates the damage value against the piercing weapons that the unit can absorb;
    * **Supplies**: indicates the quantity of food that the unit currently possesses in order to guarantee its survival. When the value reaches zero, the warrior slowly loses health until it reaches 10% of maximum health. To restore it, however, it is necessary for the soldier to be in the vicinity of any source of supply, such as villages, fortresses or units for transporting food. When the value is higher than zero, the unit will slowly recover its health until it has reached its maximum;
    * **Maximum supplies**: indicates the maximum quantity of food that the unit can take with it;
    * **Movement speed**: indicates how many meters per second the unit travels while walking;
    * **Range of action**: indicates the minimum required distance (in meters) for the unit to attack;
    * **Field of view**: indicates the diameter of the circumference of which each unit is equipped to reveal the areas covered by the fog of war;
    * **Inventory**: indicates the maximum number of objects that the unit can carry with it. The maximum ceiling is fixed to four objects;
    * **Stamina**: indicates the maximum value of energy that the unit possesses in order to activate its special abilities;
    * **Special abilities**: they are all those passive skills (whose activation, therefore, is independent of the will of the player) of which the unit is equipped and that allow him to obtain benefits in combat, or to assist the allies. These skills do not apply to heroes, who use others instead. In the next list, all are listed, with the name and its effect.
        * **Parry mode**: the unit can neither inflict damage nor receive it if it has available energy. Each failed attack consumes one point of energy, while each hit inflicted provides one;
        * **Exfoliation**: each successful hit on an opponent reduces the enemy's energy by 2 points (consumes 1 point of energy);
        * **Ferocity**: the unit recovers the maximum amount of energy when it defeats an enemy;
        * **Triple hit**: triples the damage inflicted on the enemy (consumes 1 energy point);
        * **Immunity**: allows the unit to be free from the hit inflicted by the enemy (consumes 1 point of energy);
        * **Vitality**: the warrior recovers two energy points instead of one.
        * **Absorption**: the unit receives a number of health points proportional to the damage caused to the enemy (consumes 1 energy point);
        * **Reanimation**: the warrior recovers a point of energy for every stroke inflicted on the enemy;
        * **Resistance**: the warrior receives 5 points of maximum health for each level reached;
        * **Expert attack**: adds the level of the unit to its attack value (consumes 1 energy point);
        * **Perforation**: the soldier deals maximum damage without considering the enemy's defense value (consumes 1 energy point);
        * **Damage reflected**: returns to the attacker all the damage received in the melee fight (consumes 1 energy point);
        * **Obstinacy**: deals additional damage points equivalent to 10% of the enemy's maximum health (consumes 1 energy point);
        * **Special attack**: adds to the attack value of the unit twice the energy points it currently has. Activates when the unit successfully deals a hit (consumes 1 energy point);
        * **Special defense**: adds to the unit's attack value twice the energy points it currently has. It is activated when the unit is hit (consumes 1 energy point);
        * **Fatal blow**: the unit eliminates the enemy if the level of health of the latter after receiving the blow is below 50%. This special feature has no effect on heroes (it consumes 4 energy points);
        * **Lethal Attack**: If the unit has more than twice the enemy's health points, and the enemy receives a hit, the enemy will be eliminated. You can not use against heroes (consume 4 energy points);
        * **Expert shot**: eliminates the opponent with a single blow if this is much less than him. You can not use against heroes (consume 4 energy points);
        * **Defensive tactics**: sum the level of the unit to its defense value (it consumes 1 energy point);
        * **Determination**: increase the energy by 1 every time the warrior is hit;
        * **Charge**: it is a characteristic of the mounted units, which increases the damage output by 8 times if the rider has not made an attack for more than ten seconds (consumes 4 energy points);
        * **Regeneration**: the unit recovers health more quickly (it consumes 4 energy points);
        * **Foga**: increases the ability to attack the unit that has it, to the detriment of his health. The health that loses turns into additional damage to the enemy. The loss of health of the unit that has the "Foga" is limited: it can never fall below 50% of its maximum health (it consumes 4 energy points);
        * **Revenge**: when the unit is defeated, 100 points of damage to its killer;
        * **Triumph**: the unit completely restores its health when it defeats an enemy;
        * **Venomous attack**: the warrior infects the enemy and makes him sick, taking health from him constantly, without ever falling below 10% of his maximum health. The effect of the disease disappears when the unit recovers all health (it consumes 2 energy points);
        * **Mimicry**: the unit that remains stationary for 10 seconds becomes invisible to enemies;
        * **Concealment**: allows the unit to make or render another friendly unit invisible (consumes 1 energy point);
        * **Instruction**: the warrior receives a point of experience for each stroke inflicted on an enemy with more experience (consumes 1 energy point);
        * **Initiation**: the unit that possesses this characteristic gives experience to the allied units with less experience. (Consume 1 energy point)
        * **Freedom**: the unit that possesses this characteristic can not be assigned to a hero.
        * **Vital exhaustion**: reduces the target's energy points to zero. (Consume 1 energy point)
        * **Curse**: allows the unit to direct a curse on the enemy so that he lacks the next attack. (Consume 1 energy point)
        * **Healing**: Cure the allied target unit, providing 20 health points plus one health point for each level of the unit that owns this special feature. (Consume 1 energy point)
        * **Lynx sight**: reveals the presence of enemy units hidden in the field of vision.
        * **Expansive damage**: The unit that owns it deals damage to all nearby enemies on the battlefield.
    
18. Heroes inherit all the properties of the units (health, attack, defense and so on), but they can command troops of a lower rank (minimum 50, providing special bonuses and giving them the same command simultaneously) and use some skills, active and passive, whose effectiveness increases when points are awarded (one for each level up to a maximum of 10 for each). The next list represents the specification.
    * **Prestige** (passive): for each point assigned to this characteristic, the hero can increase the number of units he commands by 2 units.
    * **Offensive superiority** (passive): for each assigned point increase by one the attack value (minimum and maximum) of the warriors commanded to the hero.
    * **Defensive superiority** (passive): each point increases the defense value (cutting and piercing) of the warriors assigned to the hero.
    * **Forced march** (passive): for each point assigned to this characteristic, the speed of movement of units bound to the hero by 5% increases.
    * **Heroic resistance** (passive): each assigned point involves an increase of 100 points in the hero's maximum health value.
    * **Erudition** (Passive): the units assigned to the hero increase by 2 levels for each point assigned to this special feature (up to a maximum of 12).
    * **Mastery** (passive): for each point assigned to this characteristic, the experience that the hero transfers to the warriors who are following him increases by 2%.
    * **Heroic attack** (passive): Increases the hero's (maximum) attack value by 5 points for each point assigned to this feature.
    * **Elusive** (passive) movement: the probability of warriors to dodge an enemy attack increases by 2% for each point assigned to this feature.
    * **Recovery** (passive): the warriors will have, for each assigned point, 10% probability of receiving 5 extra health points when they are feeding.
    * **Exploration** (passive): for each point assigned to this characteristic, the hero's field of vision progressively increases.
    * **Euphoria** (passive): when defeating an enemy, the units assigned to the hero will have 10% chance for each point assigned to this feature to get 3 points of energy.
    * **Vigor** (passive): for each assigned point increases by 10% the ability of the warriors commanded by the hero to obtain an additional energy point while feeding.
    * **Loyalty** (passive): every five seconds the hero subtracts one of his warriors, chosen at random, 20 points of health for each point assigned to this characteristic if he has an amount of health points lower than the maximum.
    * **Wisdom** (passive): when one of his warriors dies, the hero has a 5% probability for each point assigned to this feature to accumulate 1 point of experience.
    * **Defense call** (active - 4 energy points): when used, increase the unit defense by 20 points for one second, and extend the effect of 2 seconds for each point assigned to this feature.
    * **Gesture** (active - 4 energy points): when used, this feature allows warriors to ignore the defending value and defensive skills of enemies for a second, and extends the effect of as many seconds as the assigned points.
    * **Stunt** (active - 4 energy points): When used, for 5 seconds the units bound to the hero increase by as many levels as there are points assigned to this feature.
    * **Survival** (passive): every 5 seconds the soldiers have, 10% chance for each assigned point to recover health without having to feed.
    * **Charisma** (active - 6 energy points): when used add to the energy of the warriors 1 point for each point assigned to this feature.
    * **Truce** (active - 6 energy points): when used, the units will not inflict or receive any damage for as many seconds as the points assigned to this feature.
    * **Heroic healing** (active - 4 energy points): when used, the hero recovers 100 health points.
    * **Boldness** (active - 6 energy points): reduces health by 50% and doubles the attack value (minimum and maximum) of units bound to the hero. Its effect lasts one second, plus an additional second for each assigned point.
    * **Aggression** (active - 6 energy points): When used, decrease health by 5 points and add 1 point to unit energy for each point assigned to this feature.
    * **Healing** (active - 6 energy points): returns to the warriors commanded by the hero 10 health points for each point assigned to this feature.

19. When units are assigned to the hero, they will automatically be placed in formation and will continue to hold it whenever they stand still or walk: the advantage of training is that it provides bonuses to unit statistics if they are using the "keep position" command. There are four types of training, listed and examined below, with the related bonuses.
    * **Line**: units form one or more lines in front of the hero. (+2 minimum and maximum attack on sharp weapons, +2 piercing and cutting defense, + 20% minimum attack and maximum piercing weapons);
    * **Squared**: the units are arranged around the hero forming a protective ring. (+6 piercing and cutting defense);
    * **Horse-wings**: all the cavalry units are placed on the flanks of the remaining troops, which instead take on the formation "In line". (+4 minimum and maximum attack sharp weapons, + 20% minimum and maximum attack piercing weapons);
    * **In block**: the units form a compact block in front of the hero, simulating a square shield. (+4 levels);

20. The combat between the troops, of earth and of sea, can happen body to body or from distance. While melee attacks have a chance to hit the target 100%, those at a distance have a chance of 50% (to simulate the imprecision of the archers);

21. During a game, whatever the mode, you can order units to collect a magical object, which is generated at regular intervals in particular structures, called places with magical items. These objects are distinguished from each other by the power they confer and by the way in which this power can be used: in this regard, we represent the complete list.
    * **Bear teeth**: increase the maximum damage by 4 points;
    * **Herbal Amulet**: increases the piercing defense by 4 points;
    * **Force belt**: increase the sharp defense by 4 points;
    * **Rye ears**: distribute 200 units of food between the nearest friendly units;
    * **Snakeskin**: increase the minimum and maximum attack by 10 points;
    * **Snake belt**: increase the minimum and maximum attack by 30 points;
    * **Eagle feather**: increases the maximum health of 200 points;
    * **Feather amulet**: increases the maximum health of 400 points;
    * **Belt of the Kings**: increases the maximum health by 600 points and by 10 points the sharp and piercing defense;
    * **Poisonous mushrooms**: using them, permanently increase the level of the carrier by one point, at the cost of 90% of health;
    * **Medicinal herbs**: completely restores the health of the owner;
    * **Healing water**: distributes 1,000 health points among all friendly units nearby;
    * **Wild boar tusk**: adds 25 experience points. When used, it damages the opponent, subtracting health from the owner;
    * **Boar teeth**: increases the owner's level by 5 points;
    * **Victory horn**: inflicts 60 points of damage to the bearer and 12 nearby enemy units;
    * **Ashes of the druid**: they take care of their bearer and the 8 closest friendly units;
    * **Finger of death**: eliminates 3 neighboring units. It has no effect on heroes;
    * **Health gloves**: increase the health by 1,200 points. When they are used, they take care of a friendly unit, subtracting health from those who own them
    * **Fire stone**: increase the maximum attack by 60 points. When it is used, it cares for its bearer, subtracting health from a friendly unit;
    * **Bloodstone**: no effect;
    * **Gemma off**: the gem is still too weak to channel the power of the Goddess;
    * **Shining gem**: now the gem can channel the power of the Goddess;
    * **Gem of power**: the gem makes the Power of the Goddess flow in your veins;
    * **Gift of the gods**: completely heals the owner when he kills an enemy, then disappears;
    * **War booty**: adds 100 pieces of gold to the settlement where the unit that owns it is entering, then disappears;
    * **Ring of the veterans**: when the unit that carries it defeats an enemy, it turns into the medal of the veterans;
    * **Veterans' medal**: adds 6 points to the minimum and maximum attack;
