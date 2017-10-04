/**
* —ğj‚Ìî•ñ.
*/
#include"stdafx.h"
#include"HistoryInfo.h"

#include"HistoryBook\HistoryPage\HistoryPage.h"

void LocationHistoryInfo::SetData(vector<HistoryPage*>& list)
{
	for (int i = 0; i < (int)ChipID::ChipNum;i++)
	{
		if (i < list.size() && list[i] != nullptr)
			_ChipSlot[i] = list[i]->GetChipID();
		else
			_ChipSlot[i] = ChipID::None;
	}
}
