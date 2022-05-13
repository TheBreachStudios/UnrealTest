# UnrealTest

We would like you to implement a small battle arena game, in which your team competes against other teams (required) and AI controlled enemies (optional).
The test has several steps described below. You do not have to complete all the steps to the last, but the more you implement the better for the evaluation.
We will not evaluate the final art quality, but we will take into account the overall project structure (its hierarchy and naming). 
You are free to use any art resource you need but code wise you shouldn’t depend on any third-party component.
Is worth mentioning that we would like the logic to be coded in C++ instead of Blueprints as we would like to evaluate your C++ coding skills with Unreal.
Above all, we will evaluate the code architecture and quality.

1.       Project Setup

1.1     Git Workflow

We expect you to work with the next git workflow:
Create a fork from this repo.
Create your own branch from develop. After that you can manage your own commits and sub-branches as you want, but always from this created branch. In other words, do not commit directly to the already existing develop branch.
Commit often.
When you finish you can create a Pull Request to merge into our main develop branch. Here we will perform your evaluation.

2.       Game

It will be a multiplayer game. You can implement a listen server (it’s enough to be able to run the server and clients on a local network or even on the same machine running multiple client instances).
Take under consideration network latency when shooting.
To be able to simulate network issues like latency you can use Clumpsy or a similar tool. https://jagt.github.io/clumsy/download.html

2.1   Server

One of the clients will be the host and it will run one match at a time. Is up to you to define the conditions for the match to start. Either having at least a few players and starting after X seconds have passed or waiting for all the players to confirm they are ready or any other way you think will make sense. In any case you should document the way it will work.

2.2   Client

The client should be able to decide which server to connect to, you can have a simple input box for the host IP.
Once connected to the server the player will be able to select the champion to be used (required at least 1 champion, optional a variety of champions to be selected from).

2.3   Matchmaking

The strategy on how to set up the teams is up to you. The basic idea is that at least 2 teams exist when more than one player is playing. Also, it will be ideal to have at least 2 players per team. 

2.4   Gameplay

Once the match starts, all players will start in an arena. Constraints regarding the size of the arena and any props inside it will be for you to decide.
The main objective of the game is to be the last team standing.
(Optional) Also, AI controlled enemies should join the battle. They can be already there when the players join or appear in waves.

2.5   Controls

It should be a third person view.
The player should be able to move the champion using WASD, look around with the mouse and attack with the mouse left button.

2.6   Champion

You need to implement one champion with its own attack.
(Optional) It would be nice to have several champions to choose from, each of them with a different attack. You define the champion types.
(Optional) Also having different kinds of enemies to fight against.

3.       Testing
You can add some Unit Testing. You can use the Automation test in Unreal or integrate an external framework like google test or others.
 
As a bonus, if you approach your implementation in a TDD (Test-Driven Development) way from the start, even better. This will mean that you have read all the specifications before starting to implement. Remember to commit often so that we can see your progress.


## Minimum requirements

Be able to host a match and have another client that connects.

Have at least 1 champion available. (All clients can use the same)

Be able to fight against each other.

Consider network latency issues when implementing shooting.

## Bonus

Add NPCs that will fight against the player characters.

Have different champion types, with its own attack.

Add a notion of “team” and be able to play one team vs the other.
