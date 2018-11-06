#include "RIFF.h"




RIFF::RIFF(const TCHAR * filename, ImpInterface * i, Interface * gi)
{
	int count = 0;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("C:\\GAMES\\LM\\SDK\\4.3.29.25520\\Modeling\\3ds Max\\bin\\modeldef.xml");
	pugi::xml_node ModelInfo = doc.child("ModelInfo");
	SAnimation AnimTemp;
	for (pugi::xml_node Animation = ModelInfo.child("Animation"); Animation; Animation = Animation.next_sibling("Animation"))
	{
		AnimTemp.name = Animation.attribute("name").value();
		AnimTemp.guid = Animation.attribute("guid").value();

		AnimTemp.length = std::strcmp(Animation.attribute("length").value(),"") ? std::stof(Animation.attribute("length").value()) : 0.0;

		AnimTemp.type = std::strcmp(Animation.attribute("type").value(), "Standard") ? Sim : Standard;
		AnimTemp.typeParam = Animation.attribute("typeParam").value();
		AnimTemp.typeParam2 = Animation.attribute("typeParam2").value();
		AnimationXML->push_back(AnimTemp);
	}

	this->i = i;
	this->gi = gi;
	RIFFStream = new ReadStream(filename);
	RIFFText = new WriteStreamText(L"D:\\PMDG.txt");
	ReadSectionsHierarhy();
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
			WriteData(
				"material_flags				= " + std::to_string(lMAT3.material_flags) + ", \n" +
				"material_flags_2			= " + std::to_string(lMAT3.material_flags_2) + ", \n" +
				"diffuse_texture_index		= " + std::to_string(lMAT3.diffuse_texture_index) + ", \n" +
				"detail_texture_index		= " + std::to_string(lMAT3.detail_texture_index) + ", \n" +
				"bumpmap_texture_index		= " + std::to_string(lMAT3.bumpmap_texture_index) + ", \n" +
				"specular_texture_index		= " + std::to_string(lMAT3.specular_texture_index) + ", \n" +
				"emissive_texture_index		= " + std::to_string(lMAT3.emissive_texture_index) + ", \n" +
				"reflection_texture_index	= " + std::to_string(lMAT3.reflection_texture_index) + ", \n" +
				"fresnel_texture_index		= " + std::to_string(lMAT3.fresnel_texture_index) + ", \n" +
				"diffuse_color_R			= " + std::to_string(lMAT3.diffuse_color_R) + ", \n" +
				"diffuse_color_G			= " + std::to_string(lMAT3.diffuse_color_G) + ", \n" +
				"diffuse_color_B			= " + std::to_string(lMAT3.diffuse_color_B) + ", \n" +
				"diffuse_color_A			= " + std::to_string(lMAT3.diffuse_color_A) + ", \n" +
				"specular_color_R			= " + std::to_string(lMAT3.specular_color_R) + ", \n" +
				"specular_color_G			= " + std::to_string(lMAT3.specular_color_G) + ", \n" +
				"specular_color_B			= " + std::to_string(lMAT3.specular_color_B) + ", \n" +
				"specular_color_A			= " + std::to_string(lMAT3.specular_color_A) + ", \n" +
				"specular_power				= " + std::to_string(lMAT3.specular_power) + ", \n" +
				"detail_map_scale			= " + std::to_string(lMAT3.detail_map_scale) + ", \n" +
				"bump_map_scale				= " + std::to_string(lMAT3.bump_map_scale) + ", \n" +
				"reflection_scale			= " + std::to_string(lMAT3.reflection_scale) + ", \n" +
				"precipitation_offset		= " + std::to_string(lMAT3.precipitation_offset) + ", \n" +
				"specular_map_power_scale	= " + std::to_string(lMAT3.specular_map_power_scale) + ", \n" +
				"specular_bloom_floor		= " + std::to_string(lMAT3.specular_bloom_floor) + ", \n" +
				"ambient_light_scale		= " + std::to_string(lMAT3.ambient_light_scale) + ", \n" +
				"source_blend				= " + std::to_string(lMAT3.source_blend) + ", \n" +
				"destination_blend			= " + std::to_string(lMAT3.destination_blend) + ", \n" +
				"alpha_test_function		= " + std::to_string(lMAT3.alpha_test_function) + ", \n" +
				"alpha_test_threshold		= " + std::to_string(lMAT3.alpha_test_threshold) + ", \n" +
				"final_alpha_multiply		= " + std::to_string(lMAT3.final_alpha_multiply),
				parent);
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
	case (UINT('VERT')):
	{
		if (CurrVERB.VERT != NULL)
		{
			VERB->at(VERB->size() - 1).VERT = CurrVERB.VERT;
		}
		CurrVERB = { NULL,NULL,NULL };
		VERB->push_back(CurrVERB);
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
		VERB->at(VERB->size() - 1) = CurrVERB;
		//VERB->push_back(CurrVERB);
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
	case (UINT('BMAP')):
	{
		int VERBIndex = 0;
		RIFFStream->Read(&VERBIndex, Seek, sizeof(VERBIndex));
		Seek = Seek + sizeof(VERBIndex);
		SBMAP lBMAP;
		VERB->at(VERBIndex).BMAP = new std::vector<SBMAP>();
		while (Seek < EndSection)
		{
			RIFFStream->Read(&lBMAP, Seek, sizeof(lBMAP));
			Seek = Seek + sizeof(lBMAP);
			VERB->at(VERBIndex).BMAP->push_back(lBMAP);
		}
		break;
	}
	case (UINT('SKIN')):
	{
		int VERBIndex = 0;
		RIFFStream->Read(&VERBIndex, Seek, sizeof(VERBIndex));
		Seek = Seek + sizeof(VERBIndex);
		SSKIN lSKIN;
		VERB->at(VERBIndex).SKIN = new std::vector<SSKIN>();
		while (Seek < EndSection)
		{
			RIFFStream->Read(&lSKIN, Seek, sizeof(lSKIN));
			Seek = Seek + sizeof(lSKIN);
			VERB->at(VERBIndex).SKIN->push_back(lSKIN);
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
		while (Seek < EndSection)
		{
			ReadSection(parent + 1);
		}
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
		RIFFStream->Read(&lXANI, Seek, sizeof(lXANI) - 16);
		Seek = Seek + sizeof(lXANI) - 16;
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
		CurrXANI = &XANI->at(XANI->size() - 1);

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
	case (UINT('TANS')):
	{
		Seek = Temp.Size + Seek;
		break;
	}
	default:
	{
		Seek = Temp.Size + Seek;
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

