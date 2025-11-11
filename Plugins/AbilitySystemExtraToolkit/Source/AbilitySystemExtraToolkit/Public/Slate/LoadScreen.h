// SImageWithProgress.h
#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Styling/SlateBrush.h"

/**
 * 显示图片和百分比进度的 Slate 控件
 */
class ABILITYSYSTEMEXTRATOOLKIT_API  SImageWithProgress : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SImageWithProgress)
		: _Image(nullptr)
		, _Progress(0.3f)
		, _ProgressColor(FLinearColor::Green)
		, _BackgroundColor(FLinearColor::Gray)
		, _TextColor(FLinearColor::White)
		, _TextSize(16)
		, _bShowPercentage(true)
		, _ProgressBarHeight(20.0f)
		, _ImageSize(FVector2D(100.0f, 100.0f))
	{}

	/** 显示的图片 */
	SLATE_ARGUMENT(const FSlateBrush*, Image)
    
	/** 进度值 (0.0 - 1.0) */
	SLATE_ATTRIBUTE(float, Progress)
    
	/** 进度条颜色 */
	SLATE_ARGUMENT(FLinearColor, ProgressColor)
    
	/** 进度条背景颜色 */
	SLATE_ARGUMENT(FLinearColor, BackgroundColor)
    
	/** 文本颜色 */
	SLATE_ARGUMENT(FLinearColor, TextColor)
    
	/** 文本大小 */
	SLATE_ARGUMENT(int32, TextSize)
    
	/** 是否显示百分比文字 */
	SLATE_ARGUMENT(bool, bShowPercentage)
    
	/** 进度条高度 */
	SLATE_ARGUMENT(float, ProgressBarHeight)
    
	/** 图片尺寸 */
	SLATE_ARGUMENT(FVector2D, ImageSize)

	SLATE_END_ARGS()

	/** 构造控件 */
	
	void Construct(const FArguments& InArgs);

private:
	/** 获取进度文本 */
	FText GetProgressText() const;

	/** 获取进度条填充比例 */
	FOptionalSize GetProgressFillWidth() const;

private:
	// 属性
	TAttribute<const FSlateBrush* > ImageBrush;
	TAttribute<float> Progress;
	FLinearColor ProgressColor;
	FLinearColor BackgroundColor;
	FLinearColor TextColor;
	int32 TextSize;
	bool bShowPercentage;
	float ProgressBarHeight;
	FVector2D ImageSize;
};