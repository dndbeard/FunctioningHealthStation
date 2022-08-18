#include "GameAPI.h"
#include <string> 


/************************************************************
	Helper function for Health Station
*************************************************************/

// Include info and coordinates into single entity
struct Block {
	BlockInfo blockInfo;
	CoordinateInBlocks at;

};


// Return a block that is directly in front of player's hand, either on X or Y axis
Block GetCustomBlockInFrontOfHand(CoordinateInCentimeters handPosition, int offsetX = 25, int offsetY = 0) {
	Block block;
	// Get a block that is in front of player's hand
	CoordinateInCentimeters coords = CoordinateInCentimeters(
		handPosition.X + offsetX,
		handPosition.Y + offsetY,
		handPosition.Z
	);

	BlockInfo blockInfo = BlockInfo();
	CoordinateInBlocks coordsInBlocks = CoordinateInBlocks(coords);
	if (coordsInBlocks.Z < 800 && coordsInBlocks.Z > 0) {					// 0 < Z < 800 to avoid crash
		blockInfo = GetBlock(CoordinateInBlocks(coords));
	}
		
	block.at = coordsInBlocks;
	block.blockInfo = blockInfo;

	// If no custom block found, change offsets
	// Different offsets mean different sides of the block
	if (block.blockInfo.Type != EBlockType::ModBlock) {
	//if (block.blockInfo.CustomBlockID == 0) {
		if (offsetX == 25) return GetCustomBlockInFrontOfHand(handPosition, -25);
		if (offsetX == -25) return GetCustomBlockInFrontOfHand(handPosition, 0, 25);
		if (offsetY == 25) return GetCustomBlockInFrontOfHand(handPosition, 0, -25);
	}

	// Returned block can still have CustomBlockID 0 (or be invalid)!!!
	return block;
}



/************************************************************
	Config Variables (Set these to whatever you need. They are automatically read by the game.)
*************************************************************/

UniqueID HealthStationID = 823420;
UniqueID ThisModUniqueIDs[] = { HealthStationID }; // All the UniqueIDs this mod manages. Functions like Event_BlockPlaced are only called for blocks of IDs mentioned here. 

float TickRate = 1;							 // Set how many times per second Event_Tick() is called. 0 means the Event_Tick() function is never called.
int TickNum = 0;							 // Tick counter. Used to skip ticks

/************************************************************* 
//	Functions (Run automatically by the game, you can put any code you want into them)
*************************************************************/

// Run every time a block is placed
void Event_BlockPlaced(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{

}




// Run every time a block is destroyed
void Event_BlockDestroyed(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{

}


// Run every time a block is hit by a tool
void Event_BlockHitByTool(CoordinateInBlocks At, UniqueID CustomBlockID, wString ToolName, CoordinateInCentimeters ExactHitLocation, bool ToolHeldByHandLeft)
{

}

// Run X times per second, as specified in the TickRate variable at the top
void Event_Tick()
{
	CoordinateInCentimeters rightHandLocation = GetHandLocation(false);
	CoordinateInCentimeters leftHandLocation = GetHandLocation(true);

	Block blockInFrontOfHand = GetCustomBlockInFrontOfHand(rightHandLocation);

	// Handle Health Station interactions
	if (blockInFrontOfHand.blockInfo.CustomBlockID == HealthStationID) {

		// If health is full or over 100%, give a slight haptic and do nothing
		if (GetPlayerHealth() >= 1.0) {
			if (TickNum % 2 == 0) {
				SpawnHintText(blockInFrontOfHand.at + CoordinateInBlocks(0, 0, 1), L"You are at full health.", 2);
				PlayHapticFeedbackOnHand(false, 0.05, 0.1, 0.1);
			}
		}

		// If health is below 100%, heal player
		else {
			SpawnHintText(blockInFrontOfHand.at + CoordinateInBlocks(0, 0, 1), L"Healing... Please wait.", 2);
			SetPlayerHealth(0.1, true);
			PlayHapticFeedbackOnHand(false, 0.3, 0.1, 1);
		}

		TickNum++;
	}
}



// Run once when the world is loaded
void Event_OnLoad(bool CreatedNewWorld)
{

}

// Run once when the world is exited
void Event_OnExit()
{
	
}

/*******************************************************

	Advanced functions

*******************************************************/


// Run every time any block is placed by the player
void Event_AnyBlockPlaced(CoordinateInBlocks At, BlockInfo Type, bool Moved)
{

}

// Run every time any block is destroyed by the player
void Event_AnyBlockDestroyed(CoordinateInBlocks At, BlockInfo Type, bool Moved)
{

}

// Run every time any block is hit by a tool
void Event_AnyBlockHitByTool(CoordinateInBlocks At, BlockInfo Type, wString ToolName, CoordinateInCentimeters ExactHitLocation, bool ToolHeldByHandLeft)
{

}


/*******************************************************

	For all the available game functions you can call, look at the GameAPI.h file

*******************************************************/
