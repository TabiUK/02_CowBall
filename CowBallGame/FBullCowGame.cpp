#pragma once

#include "FBullCowGame.hpp"
#include <iostream>
#include <string>
#include <map>

//to make sytax Unreal friendly
#define TMap std::map 
#define TArray std::vector
//


FBullCowGame::FBullCowGame()
{
	Reset();

	// special cases - i know i am asking for trouble here
	const int32 ISOGRAM_ID = 0;
	My_Isogram_ID = ISOGRAM_ID;
	MyCurrentUsedWordLength = GetDefaultWordLength();

	return;
}


FBullCowGame::~FBullCowGame()
{
	return;
}


void FBullCowGame::Reset()
{

	const FString HIDDEN_WORD = "plane"; // this MUST be an isogram, set default to be plane
	MyHiddenWord = HIDDEN_WORD;

	const int32 MAX_HIDDEN_WORD_LENGHT = 7; // this is the maximum word length I have stored in my Word List
	MyMaxHiddenWordLenght = MAX_HIDDEN_WORD_LENGHT;

	//work out the size of the MAX_HIDDEN_WORD_LENGHT to make validation dynamic
	const FString STRING_OF_MAX = std::to_string(MAX_HIDDEN_WORD_LENGHT);
	const int32 SIZE_OF_MAX = STRING_OF_MAX.length();
	MySizeOfMax = SIZE_OF_MAX;


	const int32 MINIMUM_HIDDEN_WORD_LENGTH = 3; // this is the minimum word length I have stored in my Word List
	MyMinimumHiddenWordLength = MINIMUM_HIDDEN_WORD_LENGTH;

	const int32 DEFAULT_HIDDEN_WORD_LENGTH = 5;  // default to use
	MyDefaultHiddenWordLength = DEFAULT_HIDDEN_WORD_LENGTH;

	// add Isogram words assuming that we start from a length of 3 and end at length of 7 and all words MUST be an Isogram no validation done to list.
	// NOTE if min and max lenght change will need to add/remove words to this map
	MyIsogramWordMap.clear();
	MyIsogramWordMap[3] = { "ant", "and", "pen", "hat" };                 // 3 letter words
	MyIsogramWordMap[4] = { "walk", "park", "kite", "bike" };             // 4 letter words
	MyIsogramWordMap[5] = { "house", "light", "chair", "plane" };         // 5 letter words
	MyIsogramWordMap[6] = { "planet", "fabric", "acting", "adjust" };     // 6 letter words
	MyIsogramWordMap[7] = { "packing", "padlock", "talking", "dashing" }; // 7 letter words

	MyCurrentTry = 1;
	bGameIsWon = false;

	return;
}


int32 FBullCowGame::GetCurrentTry() const {	return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
int32 FBullCowGame::GetMinimumWordLength() const { return MyMinimumHiddenWordLength; }
int32 FBullCowGame::GetMaximumWordLength() const { return MyMaxHiddenWordLenght; }
int32 FBullCowGame::GetDefaultWordLength() const { return MyDefaultHiddenWordLength; }
int32 FBullCowGame::GetCurrentUsedWordLength() const { return MyCurrentUsedWordLength; }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }


int32 FBullCowGame::GetMaxTries() const { 
	// wordlength , max tries
	TMap<int32, int32> WordLengthToMaxTries{ {3,3}, {4,7}, {5,10}, {6,16}, {7,20} };
	return WordLengthToMaxTries[MyHiddenWord.length()];
}


EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{

	if (!IsIsogram(Guess)) {// if the guess isn't an isogram, return an error
		return EGuessStatus::Not_Isogram;
	} else if (!IsLowerCase(Guess)) {	// if the guess in't all lowercase return error
		return EGuessStatus::Not_Lowercase;
	} else if (Guess.length() != GetHiddenWordLength()) { // if the guess length is wrong return error
		return EGuessStatus::Wrong_Length;
	} else {
		return EGuessStatus::OK;
	}
	// if the guess length is wrong return error
	// otherwise return OK

	return EGuessStatus::Invalid_Status;
}


// check the word Lenght is one we know and a digit
EWordLengthStatus FBullCowGame::CheckWordLengthValidity(FString WordLength) const
{

	if (WordLength.length() == 0) return EWordLengthStatus::Default_Number;

	if ((int32)WordLength.length() > MySizeOfMax) return EWordLengthStatus::Not_Inscope;

	for (auto Letter : WordLength)
	{
		if (!isdigit(Letter)) return EWordLengthStatus::Not_Digit;
	}

	if ((int32)std::stoi(WordLength) < MyMinimumHiddenWordLength || (int32)std::stoi(WordLength) > MyMaxHiddenWordLenght) return EWordLengthStatus::Not_Inscope;


	return EWordLengthStatus::OK;
}


// receives a VALID guess, incriments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;

	// loop through all letters in the hidden word
	int32 WordLength = MyHiddenWord.length(); // assuming same length as guess

	for (int32 MHWCar = 0; MHWCar < WordLength; MHWCar++)
	{
		// compare letters against guess
		for (int32 GChar = 0; GChar < WordLength; GChar++)
		{
			// if they match
			if (Guess[GChar] == MyHiddenWord[MHWCar])
			{
				if (MHWCar == GChar) { // if they're in the same place
					BullCowCount.Bulls++; //incriment bulls
				} else {
					BullCowCount.Cows++; // must be a cows
				}
			}
		}
	}

	if (BullCowCount.Bulls == WordLength)
	{
		bGameIsWon = true;
	} else {
		bGameIsWon = false;
	}

	return BullCowCount;
}


/* assumes validation has been done before this have been called
   get next word in list until it gets to the end then rest to begining
   */
void FBullCowGame::SetIsogramWordLengthOf(int32 WordLenght)
{
	TArray<FString> IsogramArray = MyIsogramWordMap[WordLenght];

	MyHiddenWord = IsogramArray[My_Isogram_ID % IsogramArray.size()];
	if (MyCurrentUsedWordLength != WordLenght) {
		MyCurrentUsedWordLength = WordLenght;
		My_Isogram_ID = 1;
		return;
	}
	
	if ((My_Isogram_ID + 1) > (int32)IsogramArray.size()) {
		My_Isogram_ID = 0;
	} else {
		My_Isogram_ID++;
	}

}


bool FBullCowGame::IsIsogram(FString Word) const
{

	// treat 0 or 1 letter words as isogram
	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen = {};

	for (auto Letter : Word) // for all letters of the word
	{
		Letter = tolower(Letter); // handle mixed case

		if (LetterSeen[Letter]) { // check to see if we already have letter
			return false;
		} else {
			LetterSeen[Letter] = true; // add letter to map and mark it as seen.
		}
	}

	
	return true; // for example in case where /0 is entered
}


bool FBullCowGame::IsLowerCase(FString Word) const
{
	if (Word.length() < 1) { return true; }

	for (auto Letter : Word) // for all letters of the word
	{
		if (!isalpha(Letter)) continue;
		if (islower(Letter)) continue;
		return false;
	}


	return true; // for example in case where /0 is entered
}
