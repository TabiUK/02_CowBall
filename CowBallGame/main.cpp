/* This is the console executable, that makes use of the BullVow class
This acts as the view in MVC pattern, and is responsible for all
user interaction.  For game logic see the FBullCowGame class.
*/
#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.hpp"


// to make syntax Unreal friendly
using FText = std::string;
using int32 = int;


// function prototypes as outside a class
void PrintIntro();
void PlayGame();
FText GetValidGuess();
void PrintAndAskLengthOfWord();
void AskLengthOfWordAndSetWord();
bool AskToPlayAgain();
void PrintGameSummary();

FBullCowGame BCGame; // instantiate a new game, which we re-use across plays


// the entry point for our application
int main()
{
	bool bPlayAgain = false;
	do
	{
		PrintIntro();
		PlayGame();
		bPlayAgain = AskToPlayAgain();
	} while (bPlayAgain);

	return 0;
}


void PrintIntro()
{

	std::cout << "Welcome to Bulls and Cows, a fun word game.\n";
	std::cout << std::endl;
	std::cout << "   /\\____/\\   ______  " << std::endl;
	std::cout << "  |        | /      \\     __   __ " << std::endl;
	std::cout << "  | (|) (|)|/  BULL  \\   / /  / / " << std::endl;
	std::cout << "  |        |``````````` | ```` | " << std::endl;
	std::cout << "  ````\\/````   _______  | {} {}| " << std::endl;
	std::cout << "              |       \\_|      | " << std::endl;
	std::cout << "              | COW   / '''''''' " << std::endl;
	std::cout << "               ```````           " << std::endl;
	std::cout << std::endl;

	return;
}


void PrintAndAskLengthOfWord()
{

	std::cout << "Please type in a number between " << BCGame.GetMinimumWordLength() << " and " << BCGame.GetMaximumWordLength();
	std::cout << " you would like to have as your hidden isogram word lenght.\n";
	std::cout << "Pressing [Enter] will use defaul value of " << BCGame.GetCurrentUsedWordLength() << " letters in the hidden isogram word.\n";
	std::cout << std::endl;
	AskLengthOfWordAndSetWord();
	std::cout << std::endl;


	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength();
	std::cout << " letter isogram I'm thinking of?\n";

	return;
}


// ask user to type in the legnth of Isogram word they would like to guess
void AskLengthOfWordAndSetWord()
{

	FText GetWordLength = "";
	bool bIsValidNumber = false;
	int32 ValidNumer = BCGame.GetDefaultWordLength();

	do {
		std::cout << "Please enter a number from: (" << BCGame.GetMinimumWordLength() << " to " << BCGame.GetMaximumWordLength() << ") - [Enter for " << BCGame.GetCurrentUsedWordLength() << " Default] ? ";
		std::getline(std::cin, GetWordLength);

		EWordLengthStatus Status = BCGame.CheckWordLengthValidity(GetWordLength);
		if (Status == EWordLengthStatus::Default_Number)
		{
			if (BCGame.GetCurrentUsedWordLength() != BCGame.GetDefaultWordLength())
			{
				ValidNumer = BCGame.GetCurrentUsedWordLength();
			} else {
				ValidNumer = BCGame.GetDefaultWordLength();
			}
			bIsValidNumber = true;
		} else if (Status == EWordLengthStatus::OK)
		{
			ValidNumer = std::stoi(GetWordLength);
			bIsValidNumber = true;
		} else if (Status == EWordLengthStatus::Not_Inscope)
		{
			std::cout << "Please enter a number from " << BCGame.GetMinimumWordLength() << " to " << BCGame.GetMaximumWordLength() << "\n\n";
			bIsValidNumber = false;
		} else if (Status == EWordLengthStatus::Not_Digit)
		{
			std::cout << "Please enter numerical numbers only from " << BCGame.GetMinimumWordLength() << " to " << BCGame.GetMaximumWordLength() << "\n\n";
			bIsValidNumber = false;
		}
	} while (!bIsValidNumber);

	BCGame.SetIsogramWordLengthOf(ValidNumer);

	return;
}


// plays a sinle game to completion
void PlayGame()
{

	BCGame.Reset();
	PrintAndAskLengthOfWord();

	int32 MaxTries = BCGame.GetMaxTries();

	// loop for numer of time to ask for a guess while game is NOT won
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries)
	{
		FText Guess = GetValidGuess();

		// Submit valid guess to the game
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << ". Cows = " << BullCowCount.Cows << "\n\n";

	}

	PrintGameSummary();

	std::cout << std::endl;

	return;
}


// loop continually until the user gives a valid guess
FText GetValidGuess()
{
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	FText Guess = "";

	do
	{
		// get a guess from the player
		int32 CurrentTry = BCGame.GetCurrentTry();
		std::cout << "Try " << CurrentTry << " of " << BCGame.GetMaxTries() << ".";
		std::cout << " your guess : ";
		std::getline(std::cin, Guess);


		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a Guess that is without a repeating letters.\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter a Guess that is in all lower case letters.\n\n";
			break;
		default:
			// assuming the guess is valid
			break;
		}
	} while (Status != EGuessStatus::OK); // keep looping until no errors

	return Guess;
}


bool AskToPlayAgain()
{
	// prompt player if they want to play again 
	// returns true if Y or y

	std::cout << "Do you want to play again (y/n)? ";
	FText Response = "";
	std::getline(std::cin, Response);

	return  (Response[0] == 'y') || (Response[0] == 'Y');
}


void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "Well done you found the Hidden Word!\n";
	} else {
		std::cout << "Bad luck you did not manage to find the Hidden Word :(\n";
	}
}
