// Shoot Them Up Game. All Rights Reserved.

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"

void ASTUGameHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawCrosshair();
}

void ASTUGameHUD::DrawCrosshair()
{
	if (!Canvas)
	{
		return;
	}

	const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);
	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;
	const FColor LineColor = FColor::Red;

	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
