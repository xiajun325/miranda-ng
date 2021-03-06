// DbHelpers.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

typedef std::vector<const char*> SettingsList;

static int EnumSettingsProc1(const char*, void *)
{
	return 0;
}

bool ModuleSettingsExists(MCONTACT hContact, const char* pszModuleName)
{
	return db_enum_settings(hContact, EnumSettingsProc1, pszModuleName) != -1;
}

static int EnumSettingsProc2(const char *pszSetting, void *lParam)
{
	SettingsList& settingsList = *((SettingsList*)lParam);
	settingsList.push_back(_strdup(pszSetting));
	return 0;
}

void DeleteModuleSettings(MCONTACT hContact, const char* pszModuleName)
{
	SettingsList settingsList;
	int nResult = db_enum_settings(hContact, EnumSettingsProc2, pszModuleName, &settingsList);
	if (nResult != -1)
	{
		for(unsigned i=0; i<settingsList.size(); i++)
		{
			db_unset(hContact, pszModuleName, settingsList[i]);
			free((char*)settingsList[i]);
		}
	}
}


static int GetSetting(MCONTACT hContact, const char *szModule, const char *szSetting, DBVARIANT *dbv)
{
	int rr = db_get(hContact, szModule, szSetting, dbv);
	if (dbv->type != DBVT_UTF8)
		return rr;
	else
		return 1;
}

void RenameModule(MCONTACT hContact, const char* pszOldName, const char* pszNewName)
{
	SettingsList settingsList;
	int nResult = db_enum_settings(hContact, EnumSettingsProc2, pszOldName, &settingsList);
	if (nResult != -1)
	{
		DBVARIANT dbv;

		for(unsigned i=0; i<settingsList.size(); i++)
		{
			const char* pszSetting = settingsList[i];

			if (!GetSetting(hContact, pszOldName, pszSetting, &dbv))
			{
				switch (dbv.type)
				{
					case DBVT_BYTE:
						db_set_b(hContact, pszNewName, pszSetting, dbv.bVal);
					break;

					case DBVT_WORD:
						db_set_w(hContact, pszNewName, pszSetting, dbv.wVal);
					break;

					case DBVT_DWORD:
						db_set_dw(hContact, pszNewName, pszSetting, dbv.dVal);
					break;

					case DBVT_ASCIIZ:
						db_set_s(hContact, pszNewName, pszSetting, dbv.pszVal);
					break;

					case DBVT_UTF8:
						db_set_utf(hContact, pszNewName, pszSetting, dbv.pszVal);
					break;

					default:
						assert(0);
						break;
				}
				db_unset(hContact, pszOldName, pszSetting);
				db_free(&dbv);
			}
			free((char*)settingsList[i]);
		}
	}
}