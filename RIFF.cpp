#include "RIFF.h"




RIFF::RIFF(const TCHAR * filename, ImpInterface * i, Interface * gi)
{
	AnimationXML = new std::vector<SAnimation>();
	PartInfoXML = new std::vector<SPartInfo>();
	BadSec = new std::vector<UINT>();

	HKEY rKey;
	WCHAR Reget[512];
	DWORD RegetPath = sizeof(Reget);
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Lockheed Martin\\Prepar3D v4 SDK", 0, KEY_QUERY_VALUE, &rKey);
	RegQueryValueEx(rKey, L"SetupPath", NULL, NULL, LPBYTE(Reget), &RegetPath);
	RegCloseKey(rKey);
	int count = 0;
	if (Reget[lstrlen(Reget)-1] == L'\\')
	{
		Reget[lstrlen(Reget) - 1] = L'\0';
	}
	pugi::xml_document doc;
	std::wstring s = (std::wstring(Reget) + std::wstring(L"\\Modeling\\3ds Max\\bin\\modeldef.xml"));
	const wchar_t* sw = s.c_str();
	pugi::xml_parse_result result = doc.load_file((std::wstring(Reget) + std::wstring(L"\\Modeling\\3ds Max\\bin\\modeldef.xml")).c_str());
	//pugi::xml_parse_result result = doc.load_file("C:\\Users\\igor8\\Documents\\3dsMax\\scenes\\modeldef.xml");
	pugi::xml_node ModelInfo = doc.child("ModelInfo");
	
	
	for (pugi::xml_node Animation = ModelInfo.child("Animation"); Animation; Animation = Animation.next_sibling("Animation"))
	{
		SAnimation AnimTemp;
		AnimTemp.name = Animation.attribute("name").value();
		AnimTemp.guid = Animation.attribute("guid").value();

		AnimTemp.length = std::strcmp(Animation.attribute("length").value(),"") ? std::stof(Animation.attribute("length").value()) : 0.0;

		AnimTemp.type = std::strcmp(Animation.attribute("type").value(), "Standard") ? Sim : Standard;
		AnimTemp.typeParam = Animation.attribute("typeParam").value();
		AnimTemp.typeParam2 = Animation.attribute("typeParam2").value();
		AnimationXML->push_back(AnimTemp);
	}
	for (pugi::xml_node PartInfo = ModelInfo.child("PartInfo"); PartInfo; PartInfo = PartInfo.next_sibling("PartInfo"))
	{
		SPartInfo PartTemp;
		PartTemp.Name = PartInfo.child("Name").child_value();
		if (PartInfo.child("Copy"))
		{
			PartTemp.Copy = PartInfo.child("Copy").child_value();
		}
		if (PartInfo.child("AnimLength"))
		{
			PartTemp.AnimLength = std::atoi(PartInfo.child("AnimLength").child_value());
		}
		pugi::xml_node AnimXML = PartInfo.child("Animation");
		if (AnimXML)
		{
			PartTemp.AnimationF = true;
			if (AnimXML.child("Parameter").child("Code"))
			{
				PartTemp.Animation.Parameter.CodeF = true;
				PartTemp.Animation.Parameter.Code = AnimXML.child("Parameter").child("Code").child_value();
			}
			if (AnimXML.child("Parameter").child("Sim"))
			{
				PartTemp.Animation.Parameter.SimF = true;
				if (AnimXML.child("Parameter").child("Sim").child("Variable"))
				{
					PartTemp.Animation.Parameter.Sim.Variable = AnimXML.child("Parameter").child("Sim").child("Variable").child_value();
				}
				if (AnimXML.child("Parameter").child("Sim").child("Units"))
				{
					PartTemp.Animation.Parameter.Sim.Units = AnimXML.child("Parameter").child("Sim").child("Units").child_value();
				}
				if (AnimXML.child("Parameter").child("Sim").child("Scale"))
				{
					PartTemp.Animation.Parameter.Sim.ScaleF = true;
					PartTemp.Animation.Parameter.Sim.Scale = std::atof(AnimXML.child("Parameter").child("Sim").child("Scale").child_value());
				}
				if (AnimXML.child("Parameter").child("Sim").child("Bias"))
				{
					PartTemp.Animation.Parameter.Sim.BiasF = true;
					PartTemp.Animation.Parameter.Sim.Bias = std::atoi(AnimXML.child("Parameter").child("Sim").child("Bias").child_value());
				}
			}
			if (AnimXML.child("Parameter").child("Lag"))
			{
				PartTemp.Animation.Parameter.LagF = true;
				PartTemp.Animation.Parameter.Lag = atoi(AnimXML.child("Parameter").child("Lag").child_value());
			}
		}
		pugi::xml_node VisXML = PartInfo.child("Visibility");
		if (VisXML)
		{
			PartTemp.VisibleF = true;
			if (VisXML.child("Parameter").child("Code"))
			{
				PartTemp.Visible.Parameter.CodeF = true;
				PartTemp.Visible.Parameter.Code = VisXML.child("Parameter").child("Code").child_value();
			}
			if (VisXML.child("Parameter").child("Sim"))
			{
				PartTemp.Visible.Parameter.SimF = true;
				if (VisXML.child("Parameter").child("Sim").child("Variable"))
				{
					PartTemp.Visible.Parameter.Sim.Variable = VisXML.child("Parameter").child("Sim").child("Variable").child_value();
				}
				if (VisXML.child("Parameter").child("Sim").child("Units"))
				{
					PartTemp.Visible.Parameter.Sim.Units = VisXML.child("Parameter").child("Sim").child("Units").child_value();
				}
				if (VisXML.child("Parameter").child("Sim").child("Scale"))
				{
					PartTemp.Visible.Parameter.Sim.ScaleF = true;
					PartTemp.Visible.Parameter.Sim.Scale = std::atof(VisXML.child("Parameter").child("Sim").child("Scale").child_value());
				}
				if (VisXML.child("Parameter").child("Sim").child("Bias"))
				{
					PartTemp.Visible.Parameter.Sim.BiasF = true;
					PartTemp.Visible.Parameter.Sim.Bias = std::atoi(VisXML.child("Parameter").child("Sim").child("Bias").child_value());
				}
			}
			if (AnimXML.child("Parameter").child("Lag"))
			{
				PartTemp.Visible.Parameter.LagF = true;
				PartTemp.Visible.Parameter.Lag = atoi(AnimXML.child("Parameter").child("Lag").child_value());
			}
		}
		pugi::xml_node VisRXML = PartInfo.child("VisibleInRange");
		if (VisRXML)
		{
			PartTemp.VisibleInRangeF = true;
			if (VisRXML.child("Parameter").child("Code"))
			{
				PartTemp.VisibleInRange.Parameter.CodeF = true;
				PartTemp.VisibleInRange.Parameter.Code = VisRXML.child("Parameter").child("Code").child_value();
			}
			if (VisRXML.child("Parameter").child("Sim"))
			{
				PartTemp.VisibleInRange.Parameter.SimF = true;
				if (VisRXML.child("Parameter").child("Sim").child("Variable"))
				{
					PartTemp.VisibleInRange.Parameter.Sim.Variable = VisRXML.child("Parameter").child("Sim").child("Variable").child_value();
				}
				if (VisRXML.child("Parameter").child("Sim").child("Units"))
				{
					PartTemp.VisibleInRange.Parameter.Sim.Units = VisRXML.child("Parameter").child("Sim").child("Units").child_value();
				}
				if (VisRXML.child("Parameter").child("Sim").child("Scale"))
				{
					PartTemp.VisibleInRange.Parameter.Sim.ScaleF = true;
					PartTemp.VisibleInRange.Parameter.Sim.Scale = std::atof(VisRXML.child("Parameter").child("Sim").child("Scale").child_value());
				}
				if (VisRXML.child("Parameter").child("Sim").child("Bias"))
				{
					PartTemp.VisibleInRange.Parameter.Sim.BiasF = true;
					PartTemp.VisibleInRange.Parameter.Sim.Bias = std::atoi(VisRXML.child("Parameter").child("Sim").child("Bias").child_value());
				}
			}
			if (AnimXML.child("Parameter").child("Lag"))
			{
				PartTemp.VisibleInRange.Parameter.LagF = true;
				PartTemp.VisibleInRange.Parameter.Lag = atoi(AnimXML.child("Parameter").child("Lag").child_value());
			}
		}
		PartInfoXML->push_back(PartTemp);
	}
	this->i = i;
	this->gi = gi;
	RIFFStream = new ReadStream(filename);
	RIFFText = new WriteStreamText(L"D:\\PMDG.txt");
	TEXT = new std::vector<std::string>();
	MAT3 = new std::vector<SMAT3>();
	EMT1 = new std::vector<SEMT1>();
	IND3 = new std::vector<SIND3>();
	INDE = new std::vector<SINDE>();
	VERB = new std::vector<SVERB>();
	LODT = new std::vector<SLODT>();
	TRAN = new std::vector<STRAN>();
	SCEN = new std::vector<SSCEN>();
	SGAL = new std::vector<SSGAL>();
	SGBR = new std::vector<SSGBR>();
	SGBN = new std::vector<SSGBN>();
	SGVL = new std::vector<SSGVL>();
	VISI = new std::vector<SVISI>();
	AMAP = new std::vector<SAMAP>();
	XANI = new std::vector<SXANI>();
	REFP = new std::vector<SREFP>();
	ATTO = new std::vector<SATTO>();


	



	ReadSectionsHierarhy();
}
std::string RIFF::ShortCode(std::string Code)
{
	std::string sc = "";
	for (int i = 0; i < Code.length(); i++)
	{
		if ((Code[i] != ' ') &&
			(Code[i] != '\n') &&
			(Code[i] != '\t') &&
			(Code[i] != '\r'))
		{
			sc = sc + Code[i];
		}
	}
	return sc;
}

std::vector<std::string> RIFF::ConvertVisAnim(std::string str, int maxCount, int NamePar, int type)
{
	int ev = 0;
	std::vector<std::string> ReturnArr = std::vector<std::string>();
	std::string name = "";
	std::string sc = "";
	int fv = str.find('#', 0);
	if (fv == -1)
	{
		sc = ShortCode(str);
		int n1 = str.find(':');
		int n2 = str.find(',');
		name = str.substr(n1 + 1, n2 - n1 - 1);
		std::replace(name.begin(), name.end(), ' ', '_');
		std::replace(name.begin(), name.end(), ':', '_');
		ReturnArr.push_back(name);
		ReturnArr.push_back(str);
		int i;
		if (type == 0)
		{
			for (i = 0; i < PartInfoXML->size(); i++)
			{

				if (PartInfoXML->at(i).VisibleF)
				{
					if (PartInfoXML->at(i).Visible.Parameter.LagF == false)
					{
						if (std::strcmp(ShortCode(PartInfoXML->at(i).Visible.Parameter.Code).c_str(), sc.c_str()) == 0)
						{
							ReturnArr[0] = PartInfoXML->at(i).Name;
							break;
						}
					}

				}
			}
			if (i == PartInfoXML->size())
			{
				ReturnArr[0] = "custom_vis_" + ReturnArr[0];
				if (UniVISI < 10)
				{
					ReturnArr[0] = ReturnArr[0] + "_0" + std::to_string(UniVISI);
				}
				else
				{
					ReturnArr[0] = ReturnArr[0] + "_" + std::to_string(UniVISI);
				}
			}
		}
		if (type == 1)
		{
			for (i = 0; i < PartInfoXML->size(); i++)
			{
				if (PartInfoXML->at(i).AnimationF)
				{
					if (PartInfoXML->at(i).Animation.Parameter.LagF == false)
					{
						if (std::strcmp(ShortCode(PartInfoXML->at(i).Animation.Parameter.Code).c_str(), sc.c_str()) == 0)
						{
							ReturnArr[0] = PartInfoXML->at(i).Name;
							break;
						}
					}

				}
				
			}
			if (i == PartInfoXML->size())
			{
				ReturnArr[0] = "custom_anim_" + ReturnArr[0];
			}
		}
	}
	else
	{
		if (fv == 0)
		{
			int l1 = 0;
			int l2 = str.find('#', l1 + 1);
			std::string lag = str.substr(l1 + 1, l2 - l1 - 1);
			std::string code = str.substr(l2 + 1);
			sc = ShortCode(code);
			int n1 = str.find(':');
			int n2 = str.find(',');
			name = str.substr(n1 + 1, n2 - n1 - 1);
			std::replace(name.begin(), name.end(), ' ', '_');
			std::replace(name.begin(), name.end(), ':', '_');
			ReturnArr.push_back(name);
			ReturnArr.push_back(code);
			ReturnArr.push_back(lag);
			int i;
			if (type == 0)
			{
				for (i = 0; i < PartInfoXML->size(); i++)
				{

					if (PartInfoXML->at(i).VisibleF)
					{
						if (PartInfoXML->at(i).Visible.Parameter.LagF == true)
						{
							if (std::strcmp(ShortCode(PartInfoXML->at(i).Visible.Parameter.Code).c_str(), sc.c_str()) == 0)
							{
								if (PartInfoXML->at(i).Visible.Parameter.Lag == std::atof(lag.c_str()))
								{
									ReturnArr[0] = PartInfoXML->at(i).Name;
									break;
								}
							}
						}

					}
				}
				if (i == PartInfoXML->size())
				{
					ReturnArr[0] = "custom_vis_" + ReturnArr[0];
					if (UniVISI < 10)
					{
						ReturnArr[0] = ReturnArr[0] + "_0" + std::to_string(UniVISI);
					}
					else
					{
						ReturnArr[0] = ReturnArr[0] + "_" + std::to_string(UniVISI);
					}
				}
			}
			if (type == 1)
			{
				for (i = 0; i < PartInfoXML->size(); i++)
				{
					if (PartInfoXML->at(i).AnimationF)
					{
						if (PartInfoXML->at(i).Animation.Parameter.LagF == true)
						{
							if (std::strcmp(ShortCode(PartInfoXML->at(i).Animation.Parameter.Code).c_str(), sc.c_str()) == 0)
							{
								if (PartInfoXML->at(i).Animation.Parameter.Lag == std::atof(lag.c_str()))
								{
									ReturnArr[0] = PartInfoXML->at(i).Name;
									break;
								}
							}
						}

					}

				}
				if (i == PartInfoXML->size())
				{
					ReturnArr[0] = "custom_anim_" + ReturnArr[0];
				}
			}
		}
		else
		{
			std::string variable = str.substr(0, fv);
			std::string name = variable;
			std::replace(name.begin(), name.end(), ' ', '_');
			std::replace(name.begin(), name.end(), ':', '_');
			ReturnArr.push_back(name);
			ReturnArr.push_back(variable);
			int u1 = fv;
			int u2 = str.find('#', fv + 1);
			std::string unit = str.substr(u1+1, u2 - u1 - 1);
			ReturnArr.push_back(unit);
			int b1 = u2;
			int b2 = str.find('#', b1 + 1);
			std::string bias = "";
			std::string lag = "";
			if (b1 >= 0)
			{
				bias = str.substr(b1+1, b2 - b1 - 1);
				ReturnArr.push_back(bias);
				int l1 = b2;
				int l2 = str.find('#', l1 + 1);
				if (l1 >= 0)
				{
					lag = str.substr(l1+1, l2 - l1 - 1);
					ReturnArr.push_back(lag);
				}
			}
			int i;
			if (type == 0)
			{
				for (i = 0; i < PartInfoXML->size(); i++)
				{
					if (PartInfoXML->at(i).VisibleF)
					{
						if (std::strcmp((PartInfoXML->at(i).Visible.Parameter.Sim.Variable).c_str(), variable.c_str()) == 0)
						{
							if (std::strcmp((PartInfoXML->at(i).Visible.Parameter.Sim.Units).c_str(), unit.c_str()) == 0)
							{
								if (PartInfoXML->at(i).Visible.Parameter.Sim.BiasF == (ReturnArr.size() >= 4))
								{
									if (PartInfoXML->at(i).Visible.Parameter.Sim.Bias == std::atof(bias.c_str()))
									{
										if (PartInfoXML->at(i).Visible.Parameter.LagF == (ReturnArr.size() >= 5))
										{
											if (PartInfoXML->at(i).Visible.Parameter.Lag == std::atof(lag.c_str()))
											{
												ReturnArr[0] = PartInfoXML->at(i).Name;
												break;
											}
										}
									}
								}
							}
						}
					}


				}
			
				if (i == PartInfoXML->size())
				{
					ReturnArr[0] = "custom_vis_" + ReturnArr[0];
					if (UniVISI < 10)
					{
						ReturnArr[0] = ReturnArr[0] + "_0" + std::to_string(UniVISI);
					}
					else
					{
						ReturnArr[0] = ReturnArr[0] + "_" + std::to_string(UniVISI);
					}
				}
			}
			if (type == 1)
			{
				for (i = 0; i < PartInfoXML->size(); i++)
				{
					if (PartInfoXML->at(i).AnimationF)
					{
						if (std::strcmp((PartInfoXML->at(i).Animation.Parameter.Sim.Variable).c_str(), variable.c_str()) == 0)
						{
							if (std::strcmp((PartInfoXML->at(i).Animation.Parameter.Sim.Units).c_str(), unit.c_str()) == 0)
							{
								if (PartInfoXML->at(i).Animation.Parameter.Sim.BiasF == (ReturnArr.size() >= 4))
								{
									if (PartInfoXML->at(i).Animation.Parameter.Sim.Bias == std::atof(bias.c_str()))
									{
										if (PartInfoXML->at(i).Animation.Parameter.LagF == (ReturnArr.size() >= 5))
										{
											if (PartInfoXML->at(i).Animation.Parameter.Lag == std::atof(lag.c_str()))
											{
												ReturnArr[0] = PartInfoXML->at(i).Name;
												break;
											}
										}
									}
								}
							}
						}

					}

				}
				if (i == PartInfoXML->size())
				{
					ReturnArr[0] = "custom_anim_" + ReturnArr[0];
				}
			}
			
		}
	}
	return ReturnArr;
}

HRESULT RIFF::ReadSectionsHierarhy()
{
	ReadSection();
	return 0;
}

UINT RIFF::UINT2(char Name[4])
{
	return Name[3] + Name[2] * 0x100 + Name[1] * 0x10000 + Name[0] * 0x1000000;
}
void RIFF::WriteData(int Data, UINT parent)
{
	for (int i = 0; i <= parent; i++)
	{
		RIFFText->Write(&std::string(" "));
	}
	RIFFText->Write(&std::string(" "));
	RIFFText->Write(&std::to_string(Data));

}
void RIFF::WriteData(float Data, UINT parent)
{
	for (int i = 0; i <= parent; i++)
	{
		RIFFText->Write(&std::string(" "));
	}
	RIFFText->Write(&std::string(" "));
	RIFFText->Write(&std::to_string(Data));

}
void RIFF::WriteData(std::string Data, UINT parent)
{
	for (int i = 0; i <= parent; i++)
	{
		RIFFText->Write(&std::string(" "));
	}
	RIFFText->Write(&std::string(" "));
	RIFFText->Write(&Data);
	RIFFText->Write(&std::string("\n"));

}


HRESULT RIFF::ReadSection(int parent)
{
	RIFFSectionHeader Temp;
	RIFFStream->Read(&Temp, Seek, sizeof(Temp));

	//UINT name1 = UINT2(Temp.Name);
	//UINT name2 = UINT('RIFF');
	Seek = Seek + sizeof(Temp);
	UINT64 EndSection = Seek + Temp.Size;
	for (int i = 0; i <= parent; i++)
	{
		RIFFText->Write(&std::string(" "));
	}
	char* str;
	//str << Temp.Name[0] <<
	RIFFText->Write(&std::string(Temp.Name, 4));
	RIFFText->Write(&std::string("\n"));
	switch (UINT2(Temp.Name))
	{
	case (UINT('RIFF')):
	{
		if (parent == 0)
		{
			RIFFStream->Read(&RIFFData, Seek, sizeof(RIFFData));
			Seek = Seek + sizeof(RIFFData);
			for (int i = 0; i <= parent; i++)
			{
				RIFFText->Write(&std::string(" "));
			}
			RIFFText->Write(&std::string(" "));
			RIFFText->Write(&std::string(RIFFData.Version, 4));
			RIFFText->Write(&std::string("\n"));
		}
		while (Seek < EndSection)
		{
			ReadSection(parent + 1);
		}
		break;
	}
	case (UINT('MDLH')):
	{

		RIFFStream->Read(&MDLH, Seek, sizeof(MDLH));
		Seek = Seek + sizeof(MDLH);

		WriteData(std::to_string(MDLH.Magic) + ", " + std::to_string(MDLH.Version), parent);
		break;
	}
	case (UINT('RADI')):
	{

		RIFFStream->Read(&RADI, Seek, sizeof(RADI));
		Seek = Seek + sizeof(RADI);

		WriteData(std::to_string(RADI), parent);
		break;
	}
	case (UINT('TEXT')):
	{
		char str[64];
		while (Seek < EndSection)
		{
			RIFFStream->Read(&str, Seek, sizeof(str));
			Seek = Seek + sizeof(str);
			TEXT->push_back(str);
			WriteData(str, parent);
		}
		break;
	}
	case (UINT('MAT3')):
	{
		while (Seek < EndSection)
		{
			SMAT3 lMAT3;
			RIFFStream->Read(&lMAT3, Seek, sizeof(lMAT3));
			int temp = sizeof(lMAT3);
			Seek = Seek + sizeof(lMAT3);
			MAT3->push_back(lMAT3);
		}
		break;
	}
	case (UINT('MATE')):
	{
		while (Seek < EndSection)
		{
			SMAT3 lMAT3;
			RIFFStream->Read(&lMAT3, Seek, sizeof(lMAT3) - 4);
			int temp = sizeof(lMAT3) - 4;
			Seek = Seek + sizeof(lMAT3) - 4;
			lMAT3.zbias = 0.0;
			MAT3->push_back(lMAT3);
		}
			break;
		}
	case (UINT('EMT1')):
	{
		while (Seek < EndSection)
		{
			SEMT1 lEMT1;
			RIFFStream->Read(&lEMT1, Seek, sizeof(lEMT1));
			int temp = sizeof(lEMT1);
			Seek = Seek + sizeof(lEMT1);
			EMT1->push_back(lEMT1);
		}
		break;

	}
	case (UINT('IND3')):
	{
		int count = 0;
		while (Seek < EndSection)
		{
			SIND3 lIND3;
			RIFFStream->Read(&lIND3, Seek, sizeof(lIND3));
			int temp = sizeof(lIND3);
			Seek = Seek + sizeof(lIND3);
			IND3->push_back(lIND3);
			WriteData(
				std::to_string(count) + "; " +
				std::to_string(lIND3.ind1) + ", " +
				std::to_string(lIND3.ind2) + ", " +
				std::to_string(lIND3.ind3),
				parent);
			count++;
		}
		break;
	}
	case (UINT('INDE')):
	{
		int count = 0;
		while (Seek < EndSection)
		{
			SINDE lINDE;
			SIND3 lIND3;
			RIFFStream->Read(&lINDE, Seek, sizeof(lINDE));
			int temp = sizeof(lINDE);
			Seek = Seek + sizeof(lINDE);
			lIND3.ind1 = lINDE.ind1;
			lIND3.ind2 = lINDE.ind2;
			lIND3.ind3 = lINDE.ind3;
			IND3->push_back(lIND3);
			WriteData(
				std::to_string(count) + "; " +
				std::to_string(lIND3.ind1) + ", " +
				std::to_string(lIND3.ind2) + ", " +
				std::to_string(lIND3.ind3),
				parent);
			count++;
		}
		break;
	}
	case (UINT('TRAN')):
	{
		int count = 0;
		while (Seek < EndSection)
		{
			STRAN lTRAN;
			RIFFStream->Read(&lTRAN, Seek, sizeof(lTRAN));
			int temp = sizeof(lTRAN);
			Seek = Seek + sizeof(lTRAN);
			TRAN->push_back(lTRAN);
			count++;
		}
		break;
	}
	case (UINT('SCEN')):
	{
		int count = 0;
		while (Seek < EndSection)
		{
			SSCEN lSCEN;
			lSCEN.parentNode = NULL;
			RIFFStream->Read(&lSCEN, Seek, sizeof(lSCEN) - 8);
			int temp = sizeof(lSCEN) - 8;
			Seek = Seek + sizeof(lSCEN) - 8;
			lSCEN.parent = -1;
			SCEN->push_back(lSCEN);
			count++;
		}
		short child, peer, thismatrix, parent;
		short xchild, xpeer, xthismatrix, xparent;
		for (int i = 0; i < SCEN->size(); i++)
		{
			child = SCEN->at(i).child_node_index;
			peer = SCEN->at(i).peer_node_index;
			thismatrix = SCEN->at(i).amap_offset;
			parent = SCEN->at(i).parent;
			if (SCEN->at(i).child_node_index != -1)
			{
				SCEN->at(child).parent = i;
			}
			if (SCEN->at(i).peer_node_index != -1)
			{
				SCEN->at(peer).parent = SCEN->at(i).parent;
			}
		}
		break;
	}
	case (UINT('AMAP')):
	{
		int count = 0;
		while (Seek < EndSection)
		{
			SAMAP lAMAP;
			RIFFStream->Read(&lAMAP, Seek, sizeof(lAMAP));
			int temp = sizeof(lAMAP);
			Seek = Seek + sizeof(lAMAP);
			AMAP->push_back(lAMAP);
			count++;
		}
		break;
	}
	case (UINT('SGAL')):
	{
		int count = 0;
		while (Seek < EndSection)
		{
			SSGAL lSGAL;
			RIFFStream->Read(&lSGAL, Seek, sizeof(lSGAL));
			int temp = sizeof(lSGAL);
			Seek = Seek + sizeof(lSGAL);
			SGAL->push_back(lSGAL);
			count++;
		}
		break;
	}
	case (UINT('SGBR')):
	{
		int count = 0;
		while (Seek < EndSection)
		{
			SSGBR lSGBR;
			RIFFStream->Read(&lSGBR, Seek, sizeof(lSGBR));
			int temp = sizeof(lSGBR);
			Seek = Seek + sizeof(lSGBR);
			SGBR->push_back(lSGBR);
			count++;
		}
		break;
	}
	case (UINT('SGVL')):
	{
		int count = 0;
		while (Seek < EndSection)
		{
			SSGVL lSGVL;
			RIFFStream->Read(&lSGVL, Seek, sizeof(lSGVL));
			int temp = sizeof(lSGVL);
			Seek = Seek + sizeof(lSGVL);
			SGVL->push_back(lSGVL);
			count++;
		}
		break;
	}
	
	case (UINT('MDLG')):
	{

		RIFFStream->Read(&MDLG, Seek, sizeof(MDLG));
		Seek = Seek + sizeof(MDLG);
		char str[100];
		sprintf(str, "{%X-%X-%X-%X%X-%X%X%X%X%X%X}",
			MDLG.data1,
			MDLG.data2,
			MDLG.data3,
			MDLG.data4[0],
			MDLG.data4[1],
			MDLG.data4[2],
			MDLG.data4[3],
			MDLG.data4[4],
			MDLG.data4[5],
			MDLG.data4[6],
			MDLG.data4[7]
		);
		WriteData(str, parent);
		break;
	}
	case (UINT('MDLN')):
	{
		char str[256];
		RIFFStream->Read(&str, Seek, Temp.Size);
		Seek = Seek + Temp.Size;
		MDLN = str;
		WriteData(std::string(MDLN), parent);
		break;
	}
	case (UINT('BBOX')):
	{
		RIFFStream->Read(&BBOX, Seek, sizeof(BBOX));
		Seek = Seek + sizeof(BBOX);

		WriteData(
			std::to_string(BBOX.xmin) + ", " +
			std::to_string(BBOX.ymin) + ", " +
			std::to_string(BBOX.zmin) + ";"
			, parent
		);
		WriteData(
			std::to_string(BBOX.xmax) + ", " +
			std::to_string(BBOX.ymax) + ", " +
			std::to_string(BBOX.zmax) + ";"
			, parent
		);
		break;
	}
	case (UINT('MDLD')):
	{
		while (Seek < EndSection)
		{
			ReadSection(parent + 1);
		}
		break;
	}
	case (UINT('VERB')):
	{
		//CurrVERB = new SVERB();
		int count = 0;

		while (Seek < EndSection)
		{
			WriteData(
				":Part " + std::to_string(count) + "; \n"
				, 0
			);


			ReadSection(parent + 1);

			count++;
		}
		VERB->push_back(CurrVERB);
		//VERB->push_back(CurrVERB);
		break;
	}
	case (UINT('VERT')):
	{
		
		if (CurrVERB.VERT != NULL)
		{
			VERB->push_back(CurrVERB);
		}
		CurrVERB = { NULL,NULL,NULL };
		//VERB->push_back(CurrVERB);
		int count = 0;
		CurrVERB.VERT = new std::vector<SVERT>();
		while (Seek < EndSection)
		{
			SVERT lVERT;
			RIFFStream->Read(&lVERT, Seek, sizeof(lVERT));
			int temp = sizeof(lVERT);
			Seek = Seek + sizeof(lVERT);
			CurrVERB.VERT->push_back(lVERT);
			WriteData(
				std::to_string(count) + "; " +
				std::to_string(lVERT.position_x) + ", " +
				std::to_string(lVERT.position_y) + ", " +
				std::to_string(lVERT.position_z) + ", " +
				std::to_string(lVERT.normal_x) + ", " +
				std::to_string(lVERT.normal_y) + ", " +
				std::to_string(lVERT.normal_z) + ", " +
				std::to_string(lVERT.texture_mapping_x) + ", " +
				std::to_string(lVERT.texture_mapping_y),
				parent);
			count++;
		}
		break;
	}
	case (UINT('BMAP')):
	{
		int VERBIndex = 0;
		RIFFStream->Read(&VERBIndex, Seek, sizeof(VERBIndex));
		Seek = Seek + sizeof(VERBIndex);
		SBMAP lBMAP;
		CurrVERB.BMAP = new std::vector<SBMAP>();
		while (Seek < EndSection)
		{
			RIFFStream->Read(&lBMAP, Seek, sizeof(lBMAP));
			Seek = Seek + sizeof(lBMAP);
			CurrVERB.BMAP->push_back(lBMAP);
		}
		break;
	}
	case (UINT('SKIN')):
	{
		int VERBIndex = 0;
		RIFFStream->Read(&VERBIndex, Seek, sizeof(VERBIndex));
		Seek = Seek + sizeof(VERBIndex);
		SSKIN lSKIN;
		CurrVERB.SKIN = new std::vector<SSKIN>();
		while (Seek < EndSection)
		{
			RIFFStream->Read(&lSKIN, Seek, sizeof(lSKIN));
			Seek = Seek + sizeof(lSKIN);
			CurrVERB.SKIN->push_back(lSKIN);
		}
		break;
	}
	case (UINT('VISL')):
	{
		while (Seek < EndSection)
		{
			ReadSection(parent + 1);
		}
		break;
	}

	case (UINT('VISC')):
	{
		SVisible lVis;
		SVISI lVISI;
		std::string str = "";
		std::vector<char> lCode;
		std::vector<std::string> Result;
		str.resize(Temp.Size);
		RIFFStream->Read(&str[0], Seek, Temp.Size);
		Seek = Seek + Temp.Size;

		Result = ConvertVisAnim(str, 2, 2, 0);

		
		int ui = 0;
		for (ui = 0; ui < VISI->size(); ui++)
		{
			if (VISI->at(ui).CodeF)
			{
				if (VISI->at(ui).name == Result[0])
				{
					break;
				}
			}
		}
		if (ui < VISI->size())
		{
			VISI->push_back(VISI->at(ui));
		}
		else
		{
			lVISI.Default = 1;
			lVISI.CodeF = true;
			lVISI.Code = Result[1];
			lVISI.name = Result[0];
			if (Result.size() == 3)
			{
				lVISI.LagF = true;
				lVISI.Lag = std::atof(Result[2].c_str());
			}
			VISI->push_back(lVISI);
			UniVISI++;
		}
		
		break;
	}

	case (UINT('VINC')):
	{
		SVisible lVis;
		SVISI lVISI;
		std::string str = "";
		std::vector<char> lCode;
		std::vector<std::string> Result;
		str.resize(Temp.Size);
		RIFFStream->Read(&str[0], Seek, Temp.Size);
		Seek = Seek + Temp.Size;

			Result = ConvertVisAnim(str, 2, 2, 0);


			int ui = 0;
			for (ui = 0; ui < VISI->size(); ui++)
			{
				if (VISI->at(ui).CodeF)
				{
					if (VISI->at(ui).name == Result[0])
					{
						break;
					}
				}
			}
			if (ui < VISI->size())
			{
				VISI->push_back(VISI->at(ui));
			}
			else
			{
				lVISI.Default = false;
				lVISI.CodeF = true;
				lVISI.Code = Result[1];
				lVISI.name = Result[0];
				if (Result.size() == 3)
				{
					lVISI.LagF = true;
					lVISI.Lag = std::atof(Result[2].c_str());
				}
				VISI->push_back(lVISI);
				UniVISI++;
			}

		break;
	}
	case (UINT('VISS')):
	{
		SVisible lVis;
		SVISI lVISI;
		std::string str = "";
		std::vector<char> lCode;
		std::vector<std::string> Result;
		str.resize(Temp.Size);
		RIFFStream->Read(&str[0], Seek, Temp.Size);
		Seek = Seek + Temp.Size;

			Result = ConvertVisAnim(str, 4, 1, 0);

			int ui = 0;
			for (ui = 0; ui < VISI->size(); ui++)
			{
				if (VISI->at(ui).SimF)
				{
					if (VISI->at(ui).name == Result[0])
					{
						break;
					}
				}
			}
			if (ui < VISI->size())
			{
				VISI->push_back(VISI->at(ui));
			}
			else
			{
				lVISI.Default = 1;
				lVISI.SimF = true;
				lVISI.Sim.Variable = Result[1];
				lVISI.Sim.Units = Result[2];
				if (Result.size() == 4)
				{				
					lVISI.Sim.Bias = std::atof(Result[3].c_str());
				}
				if (Result.size() == 5)
				{
					lVISI.LagF = true;
					lVISI.Lag = std::atof(Result[4].c_str());
				}
				lVISI.name = Result[0];
				VISI->push_back(lVISI);
				UniVISI++;
			}

		break;
	}
	case (UINT('XAPS')):
	{
		SAnimationPart lVis;
		SVISI lVISI;
		std::string str = "";
		std::vector<char> lCode;
		std::vector<std::string> Result;
		str.resize(Temp.Size);
		RIFFStream->Read(&str[0], Seek, Temp.Size);
		Seek = Seek + Temp.Size;
		Result = ConvertVisAnim(str, 4, 1, 1);
		if (Result.size() > 0)
		{
			std::string name = "custom_anim_" + Result[0] + "_";
		}
		break;
	}
	case (UINT('XAPC')):
	{
		SAnimationPart lVis;
		SVISI lVISI;
		std::string str = "";
		std::vector<char> lCode;
		std::vector<std::string> Result;
		str.resize(Temp.Size);
		RIFFStream->Read(&str[0], Seek, Temp.Size);
		Seek = Seek + Temp.Size;
		Result = ConvertVisAnim(str, 4, 1, 1);
		if (Result.size() > 0)
		{
			std::string name = "custom_anim_" + Result[0] + "_";
		}
		break;
	}
	case (UINT('VINS')):
	{
		SVisible lVis;
		SVISI lVISI;
		std::string str = "";
		std::vector<char> lCode;
		std::vector<std::string> Result;
		str.resize(Temp.Size);
		RIFFStream->Read(&str[0], Seek, Temp.Size);
		Seek = Seek + Temp.Size;
		Result = ConvertVisAnim(str, 4, 1, 0);

		int ui = 0;
		for (ui = 0; ui < VISI->size(); ui++)
		{
			if (VISI->at(ui).SimF)
			{
				if (VISI->at(ui).name == Result[0])
				{
					break;
				}
			}
		}
		if (ui < VISI->size())
		{
			VISI->push_back(VISI->at(ui));
		}
		else
		{
			lVISI.Default = 0;
			lVISI.SimF = true;
			lVISI.Sim.Variable = Result[1];
			lVISI.Sim.Units = Result[2];
			if (Result.size() == 4)
			{
				lVISI.Sim.Bias = std::atof(Result[3].c_str());
			}
			if (Result.size() == 5)
			{
				lVISI.LagF = true;
				lVISI.Lag = std::atof(Result[4].c_str());
			}
			lVISI.name = Result[0];
			VISI->push_back(lVISI);
			UniVISI++;
		}
		break;
	}

	case (UINT('SGBN')):
	{
		SSGBN lSGBN;
		std::vector<char> lBoneName;
		lBoneName.resize(64);
		while (Seek < EndSection)
		{
			lSGBN.bone_name = "";
			RIFFStream->Read(&lBoneName[0], Seek, 64);
			Seek = Seek + 64;
			lSGBN.bone_name = std::string(&lBoneName[0]);
			SGBN->push_back(lSGBN);
		}

		break;
	}
	
	case (UINT('LODT')):
	{
		while (Seek < EndSection)
		{
			ReadSection(parent + 1);
			LODT->push_back(CurrLOD);
		}
		break;
	}
	case (UINT('LODE')):
	{
		RIFFStream->Read(&LODEData, Seek, sizeof(LODEData));
		Seek = Seek + sizeof(LODEData);
		CurrLOD.LOD = LODEData.LOD;
		CurrLOD.PART = new std::vector<SPART>();
		while (Seek < EndSection)
		{
			ReadSection(parent + 1);
		}

		break;
	}
	case (UINT('PART')):
	{
		SPART PartTemp;
		RIFFStream->Read(&PartTemp, Seek, sizeof(PartTemp));
		Seek = Seek + sizeof(PartTemp);
		CurrLOD.PART->push_back(PartTemp);
		//DrawPart(this, &PartTemp);
		break;
	}
	case (UINT('ANIB')):
	{
		UINT64 off = Temp.Size + Seek;
		//while (Seek < EndSection)
		{
			ReadSection(parent + 1);
		}
		Seek = off;
		break;
	}
	case (UINT('XANL')):
	{
		while (Seek < EndSection)
		{
			ReadSection(parent + 1);
		}
		break;
	}
	case (UINT('XANI')):
	{
		SXANI lXANI;
		RIFFStream->Read(&lXANI, Seek, sizeof(lXANI) - 16 - sizeof(SXAPS));
		Seek = Seek + sizeof(lXANI) - 16 - sizeof(SXAPS);
		lXANI.XANS = new std::vector<SXANS>();
		std::string* lS = new std::string();
		char s = ' ';
		while (int(s) != 0)
		{
			RIFFStream->Read(&s, Seek, sizeof(char));
			Seek = Seek + sizeof(char);
			//lS->append(&s);
			lS->append(&s, 1);
			//lS = lS + s;

		}
		lXANI.typeParam = lS;
		XANI->push_back(lXANI);
		CurrXANI = &(XANI->at(XANI->size() - 1));
		CurrXANI = &(XANI->at(XANI->size() - 1));

		break;
	}
	case (UINT('XANS')):
	{
		SXANS lXANS;
		RIFFStream->Read(&lXANS, Seek, sizeof(lXANS) - 8);
		Seek = Seek + sizeof(lXANS) - 8;
		lXANS.XANK = new std::vector<SXANK>();
		CurrXANI->XANS->push_back(lXANS);

		CurrXANS = &CurrXANI->XANS->at(CurrXANI->XANS->size() - 1);;

		break;
	}
	case (UINT('XANK')):
	{
		SXANKH lXANKH;
		SXANKT lXANKT;
		SXANKR lXANKR;
		SXANK lXANK = { 0 };
		RIFFStream->Read(&lXANKH, Seek, sizeof(lXANKH));
		Seek = Seek + sizeof(lXANKH);
		lXANK.type = lXANKH.type;
		lXANK.time = lXANKH.time;
		if (lXANKH.type == 4)
		{
			RIFFStream->Read(&lXANKR, Seek, sizeof(lXANKR));
			Seek = Seek + sizeof(lXANKR);
			lXANK.q0 = lXANKR.q0;
			lXANK.q1 = lXANKR.q1;
			lXANK.q2 = lXANKR.q2;
			lXANK.q3 = lXANKR.q3;
		}
		else
		{
			RIFFStream->Read(&lXANKT, Seek, sizeof(lXANKT));
			Seek = Seek + sizeof(lXANKT);
			lXANK.x = lXANKT.x;
			lXANK.y = lXANKT.y;
			lXANK.z = lXANKT.z;
		}
		CurrXANS->XANK->push_back(lXANK);
		break;
	}
	case (UINT('PLAL')):
	{
		while (Seek < EndSection)
		{
			ReadSection(parent + 1);
		}
		break;
	}
	case (UINT('REFL')):
	{
		while (Seek < EndSection)
		{
			ReadSection(parent + 1);
		}
		break;
	}
	case (UINT('REFP')):
	{
		SREFP lREFP;
		int SG;
		int SizePoint;
		RIFFStream->Read(&SG, Seek, sizeof(SG));
		Seek = Seek + sizeof(SG);
		RIFFStream->Read(&SizePoint, Seek, sizeof(SizePoint));
		Seek = Seek + sizeof(SizePoint);
		std::vector<char> lName;
		lName.resize(SizePoint);
		RIFFStream->Read(&lName[0], Seek, SizePoint);
		Seek = Seek + SizePoint;
		std::string SName(&lName[0], SizePoint);
		lREFP.scenegraph_reference = SG;
		lREFP.name = SName;
		REFP->push_back(lREFP);
		break;
	}
	case (UINT('ATTO')):
	{
		while (Seek < EndSection)
		{
			SATTO lATTO;
			int Magic1;
			int Magic2;
			short unk1;
			int unk2;
			int unk3;
			int unk4;
			int unk5;
			short type;
			short length;
			short offset;
			char fxName[80];
			RIFFStream->Read(&Magic1, Seek, sizeof(Magic1));
			Seek = Seek + sizeof(Magic1);
			int StartOff = Seek;
			RIFFStream->Read(&type, Seek, sizeof(type));
			Seek = Seek + sizeof(type);
			RIFFStream->Read(&length, Seek, sizeof(length));
			Seek = Seek + sizeof(length);
			RIFFStream->Read(&offset, Seek, sizeof(offset));
			Seek = Seek + sizeof(offset);
			RIFFStream->Read(&unk1, Seek, sizeof(unk1));
			Seek = Seek + sizeof(unk1);
			RIFFStream->Read(&unk2, Seek, sizeof(unk2));
			Seek = Seek + sizeof(unk2);
			RIFFStream->Read(&unk3, Seek, sizeof(unk3));
			Seek = Seek + sizeof(unk3);
			RIFFStream->Read(&unk4, Seek, sizeof(unk4));
			Seek = Seek + sizeof(unk4);
			RIFFStream->Read(&unk5, Seek, sizeof(unk5));
			Seek = Seek + sizeof(unk5);
			RIFFStream->Read(&fxName, Seek, sizeof(fxName));
			Seek = Seek + sizeof(fxName);
			std::vector<char> lParam;
			lParam.resize(StartOff + offset - Seek);
			RIFFStream->Read(&lParam[0], Seek, StartOff + offset - Seek);
			Seek = Seek + StartOff + offset - Seek;
			std::vector<char> lName;
			lName.resize(length - offset);
			RIFFStream->Read(&lName[0], Seek, length - offset);
			Seek = Seek + length - offset;
			std::string SFX(fxName);
			std::string SParam(&lParam[0]);
			std::string SName(&lName[0]);
			RIFFStream->Read(&Magic2, Seek, sizeof(Magic2));
			Seek = Seek + sizeof(Magic2);
			lATTO.attachPoint = SName;
			lATTO.fxName = SFX;
			lATTO.length = length;
			lATTO.Magic1 = Magic1;
			lATTO.Magic2 = Magic2;
			lATTO.offAttachPoint = offset;
			lATTO.params = SParam;
			lATTO.type = type;
			lATTO.unk1 = unk1;
			lATTO.unk2 = unk2;
			lATTO.unk3 = unk3;
			lATTO.unk4 = unk4;
			lATTO.unk5 = unk5;
			ATTO->push_back(lATTO);
		}
		break;
	}
	case (UINT('TANS')):
	{
		Seek = Temp.Size + Seek;
		break;
	}
	default:
	{
		Seek = Temp.Size + Seek;
		bool bf = false;
		for (int i = 0; i < BadSec->size(); i++)
		{
			if (BadSec->at(i) == UINT2(Temp.Name))
			{
				bf = true;
			}
		}
		if (!bf)
		{
			MessageBoxExA(0, (std::string("Unknown section: ") + std::string(Temp.Name, 4)).c_str(), "Warning", MB_OK, 0);
			BadSec->push_back(UINT2(Temp.Name));
		}
		break;
	}
	}
	return 0;
	}

	RIFF::~RIFF()
	{
		if (RIFFStream)
		{
			delete RIFFStream;
			RIFFStream = NULL;
		}
		if (RIFFText)
		{
			delete RIFFText;
			RIFFText = NULL;
		}
	}

