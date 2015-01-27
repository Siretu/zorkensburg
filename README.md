# Zorkensburg

### Introduction

This project was created for an course in program construction in C++ by [@Siretu](https://github.com/user/Siretu) and [@raindirve](https://github.com/user/raindirve)

### Installation

Clone the repo and run make.

### Comments/Warnings

* The combat and damage system was tacked on at the end and is not pretty or modular. It pretty much just supports the example worldfile.
* Creating and removing items using the scripting language in the world file is not fully functional. It can cause issues and depends on where the item is and the containers around it.

### Features

Zorkensburg features features a very customizable worldfile format which allows for custom commands and events to be scripted right into the worldfile without having to recompile the code. This is done by anonymous functions being added to an actor's event vector, which is checked for commands when a user types a command. More information about this format and how it's written can be found in the section below. It also features loading and saving to this worldfile format, with the name you choose.

Zorkensburg contains a reasonably simple example taken almost entirely from the start of Zork. This is used to showcase several different features of the system. Features such as:
* Easy multi-locked passages (Requiring several commands before you can go through. For example unlocking a door and then opening it)
* Aliases (Makes it simple to create another name for an existing command, such as "i" for inventory)
* Simple events, with a structure that makes it easy to add your own.
* A light system, separating light and dark rooms and making it impossible to see anything in a room lacking a light source.

### Worldfile

The worldfile is a list of rows, each containing a tag and a bunch of parameters. The parameters are separated by semi-colons. The semi-colons has to be there for each parameter that the tag needs, even if you don't want to specify a certain parameter. Some rows can be blank and some rows might also contain an endtag to end certain blocks of content. This section goes through them all:

* INTRODUCTION:

Contains only one parameter, the message to print at the beginning of the game.

* ROOM:

Contains two parameters, the description of the room and an integer 0 or 1 (boolean) that describes if the room is dark or not.

* PLAYER:

Players are a type of character and all characters end with an ENDCHAR end-tag on a later row. Characters are placed into the last room tag above it. Items or containers between the character and the character's end tag are added to the character's inventory. The Player has four parameters. The first is the name and is generally not used. The second are any potential flags the player has (see the scripting section below for more information). The third is the player description and the last one is the health.

* NPC:

An NPC is a non-playable character and is just like a PC except it's not controlled by a real player. These can take many forms but is in the example a troll. Check the Player description for information about its parameters.

* ITEM: 

An item can be picked up and moved around. It has four parameters. The first one is the name. You can specify several different names that all refer to the same object. You can do this by separating them with a hashtag (#). When using an item as a target in scripts, you have to specify the first name if you only have one name, or the second if you have more than one. The second parameter is the flags (See the scripting section below). The third parameter is the description and the fourth parameter is if the item is hidden or not.

* CONTAINER: 

The container can contain items. This worked even with nested containers. It has the same parameters as an item, but it also comes with an ENDCONT tag. Every item after the container tag and before the next endcont tag will end up in this container.

* DOOR:

Doors symbolize the passage between two rooms. They have 8 parameters. The first one is the name/id that can be used to refer to it by script. The second parameter is the description. The third parameter is the flags. The door links the rooms written in parameter four and five together. Parameters six and seven describe the directions used to go between them. The last parameter is the locked message. This is the message displayed if the player tries to go in the direction of a door, but it's locked. If this message is just an empty string, the door is considered unlocked and the player will go through.

* EVENT:

Events are part of the scripting and are explained in detail in the next section.

* DEFINE:

Define is used to define aliases. It takes two parameters. It makes it so the first parameter can be used as a command instead of the second parameter. Useful if you've set-up scripting for a custom command and don't want to repeat it for every synonym. 


### Scripting

Scripting is based on the EVENT-tag. Each EVENT-row contains an event that causes it, a condition to check and if the condition is true, it will run a bunch of actions.

#### Flags

One very important part of the scripting is the flag system. Flags are just strings that can be stored in an actor to save states. It's mainly used for conditions but can be used for other things, like the lighting as well. All actors in the game can have a set of flags. When an event is triggered, it will check if the specified actor has those flags. If it does it will run the action.

#### Events

There are two types of events. The first type is a normal command and allows you to define custom behavior with commands that you don't want to hardcode. Example: pull carpet. In the EVENT-tag, the event parameters are the first three of the parameters on the line. The first is the name of the command (e.g pull). The second is the id of the actor that the command targets and the third is the id of the actor that is supposed to be checked. This actor is the one that the condition checks the flags on and this is the actor that will be targeted by the actions. 

So if you want a light that turns on and off when you click a button, you'd want the event to look something like: EVENT:push;button;light

#### Conditions

Conditions is the fourth parameter in the event tag. It is a boolean expression that has to be true. It can contain names of flags, "!" for negation, "&" for boolean and and "|" for boolean or. A flag is considered true if the actor in the third argument has the flag. Example condition: "buttonpushed&!leverpulled" (if the button is pushed and the lever is not pulled:).

#### Actions

Actions are run when an event is triggered if and only if the conditions evauluates to true. One thing worth noting is that the conditions will be evaluated separately for each action and each action will be executed it in the order it's written in the worldfile. This is important since if you have an action early on that changes a flag in the condition, it can cause the condition to evaluate to false, making the rest of the actions not run.

There are several actions and it's easy to add more. Each action is separated by semi-colons. An action starts with the action name in capital letters and any additional parameters are added after it, each separated by a hashtag.

Here are all the currently implemented actions:

* Message: 

This action simply prints a message in the game. It only has one parameter, the message to print.

* Set:

This action sets a flag on the target specified. 

* Unset:

This action unsets a flag on the target specified.

* Lockmessage:

This action changes the targeted door's lockmessage. This can be used to lock doors or unlocking them (by setting it to an empty string).

* Createitem:

Creates an item. Takes three parameters, the name, the description and if it's hidden or not.

* Destroyitem:

Destroys the specified item.

* Failmessage:

This action is special and is run only if the conditions evaluate to false. It's a way to print something if the event fails due to the condition.

* Follow:

Small hack for the troll AI. When executed, looks if the player is in a neighboring room and in that case moves to that room.

* Attack:

Attacks the target.

* Quit:

Quits the game.

#### Special events

The events described before are bound to commands written by the player. There are currently two other special events that can be used for other interesting features.

* onEnter:

onEnter takes a room name and is run when someone enters that room. In the example, it is used to automatically lock the door behind the player when the player enters a certain room.

* onAction:

Each character gets the chance to do an action each turn. This event can be used to decide what an NPC does on it's turn.

#### Cool stuff

##### Lightning

A room is considered dark if it was created as dark and there's no actor acting as a lightsource in the room. The lightsource mechanic is integrated with the flags and anything with the "lightsource" flag on it will light up the room. This makes it easy to implement lightning logic in the worldfile if one would like to.

##### AI

The Troll AI is currently implemented mostly with scripting. Using a small hack that makes it so all actors get the "playerishere" flag if the unit is in the same room and a special "follow" action to follow the player around, it was possible to implement a decent AI. The Troll stays in its room until the player comes in, at which point the troll is "activated" by setting the active flag. Then if the player leaves the room, it will follow, otherwise it will attack.
