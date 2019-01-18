/* The hame logic (no view code or direct user interaction)
The game is a simple guess the word game based on MasterMind
*/
#pragma once
#include <string>
#include <map>
#include <vector>

//to make sytax Unreal friendly
#define TMap std::map 
#define TArray std::vector

using FString = std::string;
using int32 = int;
//


// all values intialised to zero
struct FBullCowCount
{

	int32 Bulls = 0;
	int32 Cows = 0;
};


// returned from CheckGuessValidity, will check that Guessed Word is correctly typed
enum class EGuessStatus
{
	Invalid_Status,
	OK,
	Not_Isogram,
	Wrong_Length,
	Not_Lowercase
};


// returned from CheckWordLengthValidity, will check that Word Length requested is within values expected
enum class EWordLengthStatus
{
	Invalid_Status,
	OK,
	Not_Digit,
	Not_Inscope,
	Default_Number // this repersent that we got no word in so default to preset values
};


class FBullCowGame
{

public:
	FBullCowGame();
	~FBullCowGame();

	int32 GetMaxTries() const;
	int32 GetCurrentTry() const;
	int32 GetHiddenWordLength() const;
	int32 GetMinimumWordLength() const;
	int32 GetMaximumWordLength() const;
	int32 GetDefaultWordLength() const;
	int32 GetCurrentUsedWordLength() const;
	bool IsGameWon() const;

	EGuessStatus CheckGuessValidity(FString) const;
	EWordLengthStatus CheckWordLengthValidity(FString) const;

	void Reset(); 
	FBullCowCount SubmitValidGuess(FString);
	void SetIsogramWordLengthOf(int32);

private:

	// see constructor
	int32 MyCurrentTry;
	int32 MyMaxHiddenWordLenght;
	int32 MyMinimumHiddenWordLength;
	int32 MyDefaultHiddenWordLength;
	int32 MySizeOfMax;
	FString MyHiddenWord;
	bool bGameIsWon;

	// Isogram letter length -> array of Isogram word of x length
	TMap<int32, TArray<FString>> MyIsogramWordMap;

	// used to keep track of which array element we are using
	int32 My_Isogram_ID;
	// if recieve a new word length reset My_Isogram_ID
	int32 MyCurrentUsedWordLength;


	bool IsIsogram(FString) const;
	bool IsLowerCase(FString) const;



}; // FBullCowGame