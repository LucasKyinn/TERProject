# TERProject

**Engine version : Unreal Engine 4.27**

I'm working on porting and improving this project to UE5.

End of year school project from 2021.

**Video :**
Français avec les explications :

[![image](https://user-images.githubusercontent.com/78411295/213878126-9492bd39-f061-4613-9fd3-e3d366e3b6bf.png)
](https://youtu.be/wQ0BMB47UrY)

[![image](https://user-images.githubusercontent.com/78411295/213880575-c97eef76-aa54-4208-bb67-48d522138668.png)
](https://youtu.be/DWJ6n91Ic00)

## Introduction to reinforcement learning

Reinforcement learning is a approach used when there is no dataset, data is obtained from within the environment in which an actor operates.
The actor will be able to interact with its environment and recover data to build its own dataset. The agent, which is the actor equipped with the reinforcement algorithm, will then be able to use its dataset to take actions that will be beneficial in the long term, it will be able to optimize the number of rewards attributed by the environment.

The goal of reinforcement learning is therefore to maximize the number of rewards where the goal of supervised learning algorithms is to minimize an error function.

The notions that are important to understand here are the notions of **agent**, **state**, **action** and **rewards**.

The **agent** is an actor inside the environment that has the ability to interact with it through predefined **actions**.
This agent is constantly in a **state** that is defined by variables that the agent retrieves from the environment, this state will vary according to the actions taken by the agent. 
Finally, each change in state will result in the attribution of a **reward** to the agent by the environment, these rewards are a way of judging whether the change in state has been beneficial or not for the agent.

[![image](https://user-images.githubusercontent.com/78411295/210243969-1db0cb73-49f6-4111-93bc-5dd99b5fe0b2.png)](https://youtu.be/otRM-jXEboI)

### The Agent
  A fully Animated German Shepherd from GiM [Animalia - German Shepherd](https://www.unrealengine.com/marketplace/en-US/product/animalia-german-shepherd-m/reviews). 

### The State

  The state is defined by :
  
  • A Boolean representing whether the player is dead. 
  
  • A Boolean indicating whether the agent has low hit points (HP). 
  
  • A Boolean representing whether the agent is dead. 
  
  • An Integer representing the number of enemies attacking the player. 
  
  • An Integer representing the number of enemies attacking the agent. 
   
### The Actions

  • Attack
  
  • Heal
  
  • Flee
  
### The rewards

  • -100 if the player or agent dies.
  
  • +2 if the agent exits the critical health zone.
  
  • -1 if it remains in this zone.
  
  • +1 or -1 depending on the number of zombies attacking the player.
  
  • +5 if the total number of zombies attacking decreases.

Since training the AI would take to much time I aslo made a smaller scale experiment in order to showcase the AI learning.

## Experiment 

[![image](https://user-images.githubusercontent.com/78411295/210245575-89be5b27-f179-40d0-a45c-a0f265127fd5.png)](https://youtu.be/e9yKYpQXrzY)
 
![image](https://user-images.githubusercontent.com/78411295/210246710-af94c609-f6f4-4dfc-b689-67832c6eeecb.png)

## Full report

[RapportTERLucasREALE.pdf](https://github.com/LucasKyinn/TERProject/files/10332102/RapportTERLucasREALE.pdf) (**French**)
