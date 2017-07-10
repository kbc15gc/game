/**
* —ğj‚Ìî•ñ.
*/
#include"stdafx.h"
#include"HistoryInfo.h"

#include"HistoryBook\HistoryPage\HistoryPage.h"

void LocationHistoryInfo::SetData(vector<HistoryPage*>& list)
{
	int i = 0;
	for (auto it : list)
	{
		if (it != nullptr)
		{
			_ChipSlot[i++] = it->GetChipID();
		}
	}
}
