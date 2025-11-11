#include "LoadScreen.h"

// SImageWithProgress.cpp

#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"

void SImageWithProgress::Construct(const FArguments& InArgs)
{
    ImageBrush = InArgs._Image;
    Progress = InArgs._Progress;
    ProgressColor = InArgs._ProgressColor;
    BackgroundColor = InArgs._BackgroundColor;
    TextColor = InArgs._TextColor;
    TextSize = InArgs._TextSize;
    bShowPercentage = InArgs._bShowPercentage;
    ProgressBarHeight = InArgs._ProgressBarHeight;
    ImageSize = InArgs._ImageSize;

    // 创建子控件
    TSharedPtr<SWidget> ProgressWidget;
    
    if (bShowPercentage)
    {
        // 包含文本的进度条
        ProgressWidget = 
            SNew(SOverlay)
            + SOverlay::Slot()
            [
                // 进度条背景
                SNew(SBox)
                .HeightOverride(ProgressBarHeight)
                .WidthOverride(ImageSize.X)
                [
                    SNew(SImage)
                    .ColorAndOpacity(BackgroundColor)
                ]
            ]
            + SOverlay::Slot()
            [
                // 进度条填充
                SNew(SBox)
                .HeightOverride(ProgressBarHeight)
                .WidthOverride(this, &SImageWithProgress::GetProgressFillWidth)
                [
                    SNew(SImage)
                    .ColorAndOpacity(ProgressColor)
                ]
            ]
            + SOverlay::Slot()
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            [
                // 进度文本
                SNew(STextBlock)
                .Text(this, &SImageWithProgress::GetProgressText)
                .ColorAndOpacity(TextColor)
                .Font(FCoreStyle::GetDefaultFontStyle("Regular", TextSize))
            ];
    }
    else
    {
        // 只有进度条，没有文本
        ProgressWidget = 
            SNew(SOverlay)
            + SOverlay::Slot()
            [
                SNew(SBox)
                .HeightOverride(ProgressBarHeight)
                .WidthOverride(ImageSize.X)
                [
                    SNew(SImage)
                    .ColorAndOpacity(BackgroundColor)
                ]
            ]
            + SOverlay::Slot()
            [
                SNew(SBox)
                .HeightOverride(ProgressBarHeight)
                .WidthOverride(this, &SImageWithProgress::GetProgressFillWidth)
                [
                    SNew(SImage)
                    .ColorAndOpacity(ProgressColor)
                ]
            ];
    }

    // 主控件布局
    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .AutoHeight()
        .HAlign(HAlign_Center)
        [
            // 图片
            SNew(SBox)
            .WidthOverride(ImageSize.X)
            .HeightOverride(ImageSize.Y)
            [
                SNew(SImage)
                .Image(ImageBrush)
            ]
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(0.0f, 5.0f, 0.0f, 0.0f)
        .HAlign(HAlign_Center)
        [
            // 进度条
            ProgressWidget.ToSharedRef()
        ]
    ];
}

FText SImageWithProgress::GetProgressText() const
{
    const float CurrentProgress = Progress.Get();
    const int32 Percentage = FMath::RoundToInt(CurrentProgress * 100.0f);
    return FText::Format(NSLOCTEXT("ImageWithProgress", "Percentage", "{0}%"), Percentage);
}

FOptionalSize SImageWithProgress::GetProgressFillWidth() const
{
    const float CurrentProgress = Progress.Get();
    const float ClampedProgress = FMath::Clamp(CurrentProgress, 0.0f, 1.0f);
    return ImageSize.X * ClampedProgress;
}