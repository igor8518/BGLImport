//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "MDLConverterv2.h"

#define MDLConverterv2_CLASS_ID	Class_ID(0x9a4c648d, 0x54080edb)


class MDLConverterv2 : public SceneImport
{
public:
	//Constructor/Destructor
	MDLConverterv2();
	virtual ~MDLConverterv2();

	virtual int				ExtCount();					// Number of extensions supported
	virtual const TCHAR *	Ext(int n);					// Extension 0n (i.e. "3DS")
	virtual const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	virtual const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	virtual const TCHAR *	AuthorName();				// ASCII Author name
	virtual const TCHAR *	CopyrightMessage();			// ASCII Copyright message
	virtual const TCHAR *	OtherMessage1();			// Other message 01
	virtual const TCHAR *	OtherMessage2();			// Other message 02
	virtual unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	virtual void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
	virtual int				DoImport(const TCHAR *name,ImpInterface *i,Interface *gi, BOOL suppressPrompts=FALSE);	// Import file
};



class MDLConverterv2ClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new MDLConverterv2(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_IMPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return MDLConverterv2_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("MDLConverterv2"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetMDLConverterv2Desc() { 
	static MDLConverterv2ClassDesc MDLConverterv2Desc;
	return &MDLConverterv2Desc; 
}




INT_PTR CALLBACK MDLConverterv2OptionsDlgProc(HWND hWnd,UINT message,WPARAM ,LPARAM lParam) {
	static MDLConverterv2* imp = nullptr;

	switch(message) {
		case WM_INITDIALOG:
			imp = (MDLConverterv2 *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}


//--- MDLConverterv2 -------------------------------------------------------
MDLConverterv2::MDLConverterv2()
{

}

MDLConverterv2::~MDLConverterv2() 
{

}

int MDLConverterv2::ExtCount()
{
	#pragma message(TODO("Returns the number of file name extensions supported by the plug-in."))
	return 1;
}

const TCHAR *MDLConverterv2::Ext(int /*n*/)
{		
	#pragma message(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	return _T("MDL");
}

const TCHAR *MDLConverterv2::LongDesc()
{
	#pragma message(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\")"))
	return _T("Flight Simulator X MDL File");
}
	
const TCHAR *MDLConverterv2::ShortDesc() 
{			
	#pragma message(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("RIFF Model");
}

const TCHAR *MDLConverterv2::AuthorName()
{			
	#pragma message(TODO("Return ASCII Author name"))
	return _T("Igor M.");
}

const TCHAR *MDLConverterv2::CopyrightMessage() 
{	
	#pragma message(TODO("Return ASCII Copyright message"))
	return _T("");
}

const TCHAR *MDLConverterv2::OtherMessage1() 
{		
	//TODO: Return Other message 01 if any
	return _T("");
}

const TCHAR *MDLConverterv2::OtherMessage2() 
{		
	//TODO: Return other message 02 in any
	return _T("");
}

unsigned int MDLConverterv2::Version()
{				
	#pragma message(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return 020;
}

void MDLConverterv2::ShowAbout(HWND /*hWnd*/)
{			
	// Optional
}

int Parts = 0;

double deg2rad(double degrees) {
	return degrees * 4.0 * atan(1.0) / 180.0;
}
#define KEY_POS		3
#define KEY_ROT		4



void SetControllerKeys(Control *cont, SXANK *keys, int type, int ctt, int ct)
{
	SXANK *ptr = keys;
	//int ct = 0;
	ILinPoint3Key pkey;
	ILinRotKey rkey;
	IKey *k;

	// Set up 'k' to point at the right derived class
	switch (type) {
	case KEY_POS: k = &pkey; break;
	case KEY_ROT: k = &rkey; break;
	default: return;
	}

	// Count how many keys we got.
	//ct = 1;

	// Special case where there is only one key at frame 0.
	// Just set the controller's value
	if (ct == 2 && keys->time == 0) {
		switch (type) {

		case KEY_POS: {
			Point3 p(
				keys->x,
				keys->y,
				keys->z);
			cont->SetValue(0, &p);
			break;
		}
		case KEY_ROT: {
			Quat q(
				keys->q0,
				keys->q1,
				keys->q2,
				keys->q3
			);
			cont->SetValue(0, &q);
			break;
		}
		}
		return;
	}
	// Make sure we can get the interface and have some keys
	IKeyControl *ikeys = GetKeyControlInterface(cont);
	if (!ikeys) return;

	// Allocate the keys in the table
	ikeys->SetNumKeys(ctt);

	ptr = keys;
	//while (ptr) 
	
	{
		// Set the common values
		k->time = ptr->time * GetTicksPerFrame();

		// Set the key type specific values
		switch (type) {

		case KEY_POS:
			pkey.val[0] = ptr->x;
			pkey.val[1] = ptr->y;
			pkey.val[2] = -ptr->z;
			break;

		case KEY_ROT:
			rkey.val[3] = -ptr->q3;
			rkey.val[2] = ptr->q2;
			rkey.val[1] = -ptr->q1;
			rkey.val[0] = -ptr->q0;

			
			



			break;

		}

		// Set the key in the table
		ikeys->SetKey(ct, k);
	}

	// This will ensure that the keys are sorted by time at the
	// track is invalidated so the tangents will be recomputed.
	//ikeys->SortKeys();
}

INode* OneNode = NULL;
INode* BoneNode = NULL;
////////////////////////
Matrix3 tmO = Matrix3(1);


Matrix3 tmR = Matrix3(1);


struct SParts
{
	INode* node;
	int LOD;
	int Part;
	std::vector<int>* Inds;
};
std::vector<SParts>* NodeParts = new std::vector<SParts>();
std::vector<INode*>* Bones = new std::vector<INode*>();
std::vector<StdMat2*>* Mtls = new std::vector<StdMat2*>();
int NodeIndex = -1;
std::wstring StartUD = L"FSXML = <?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?> <FSMakeMdlData version=\"9.0\"> ";
std::wstring EndUD = L" </FSMakeMdlData>";

std::string BytesToGUID(byte b[16])
{
	char ss[37];
	sprintf(ss, "% 2.2X% 2.2X% 2.2X% 2.2X-% 2.2X% 2.2X-% 2.2x% 2.2x-% 2.2X% 2.2X-% 2.2X% 2.2X% 2.2X% 2.2X% 2.2X% 2.2X",
		b[3],
		b[2],
		b[1],
		b[0],
		b[5],
		b[4],
		b[7],
		b[6],
		b[8],
		b[9],
		b[10],
		b[11],
		b[12],
		b[13],
		b[14],
		b[15]);
	ss[36] = '\0';
	return std::string(ss, 37);
}


std::wstring SetAnim(RIFF *riff, INode* node, int CurrNode)
{

	int xanimInd = riff->SGAL->at(riff->SCEN->at(CurrNode).amap_offset / 8).xanim;
	std::wstring nameAnim = L"";
	if (xanimInd >= 0)
	{
		Control* c;
		c = node->GetTMController()->GetPositionController();
		if (c && c->ClassID() != Class_ID(LININTERP_POSITION_CLASS_ID, 0)) {
			Control *tcb = (Control*)riff->gi->CreateInstance(
				CTRL_POSITION_CLASS_ID,
				Class_ID(LININTERP_POSITION_CLASS_ID, 0));
			if (!node->GetTMController()->SetPositionController(tcb)) {
				tcb->DeleteThis();
			}
		}

		c = node->GetTMController()->GetRotationController();
		if (c && c->ClassID() != Class_ID(LININTERP_ROTATION_CLASS_ID, 0)) {
			Control *tcb = (Control*)riff->gi->CreateInstance(
				CTRL_ROTATION_CLASS_ID,
				Class_ID(LININTERP_ROTATION_CLASS_ID, 0));
			if (!node->GetTMController()->SetRotationController(tcb)) {
				tcb->DeleteThis();
			}
		}
		nameAnim = L"NA";
		for (int i = 0; i < riff->XANI->size(); i++)
		{

			for (int j = 0; j < riff->XANI->at(i).XANS->size(); j++)
			{
				SXANS xans = riff->XANI->at(i).XANS->at(j);
				if (xanimInd == riff->XANI->at(i).XANS->at(j).animation_ID)
				{

					if (node->GetTMController())
					{
						if (nameAnim == L"NA")
						{
							SuspendAnimate();
							AnimateOn();
							std::string ss = BytesToGUID(riff->XANI->at(i).guid);

							for (int jj = 0; jj < riff->AnimationXML->size(); jj++)
							{
								if (std::strcmp(strlwr((char*)ss.c_str()), strlwr((char*)riff->AnimationXML->at(jj).guid.c_str())) == 0)
								{
									nameAnim = std::wstring(riff->AnimationXML->at(jj).name.begin(), riff->AnimationXML->at(jj).name.end());
									break;
								}
							}
							/////////////////////////////////////


							Control *TControl = node->GetTMController();
							DefNoteTrack* nt = new DefNoteTrack();
							TControl->AddNoteTrack(nt);
							nt->AddNewKey(0, 0);
							nt->AddNewKey(0, 0);
							NoteKeyTab keys;
							keys.Init();
							std::wstring anStart = L"ANIM_START = \"" + nameAnim + L"\"";
							std::wstring anEnd = L"ANIM_END = \"" + nameAnim + L"\"";
							NoteKey* nk = new NoteKey(0, anStart.c_str());
							nk->time = 0;
							keys.Append(1, &nk);

							nk = new NoteKey(0, anEnd.c_str());

							nk->time = riff->XANI->at(i).animation_length * GetTicksPerFrame();
							keys.Append(1, &nk);

							nt->keys = keys;
						}
						////////////////////////////////////////


						for (int k = 0; k < xans.XANK->size(); k++)
						{
							if (xans.XANK->at(k).type == 4)
							{
								Control *rotControl = node->GetTMController()->GetRotationController();
								if (rotControl)
								{
									SetControllerKeys(
										rotControl,
										&xans.XANK->at(k),
										KEY_ROT,
										xans.XANK->size(), k);

								}

							}
							else
							{
								Control *posControl = node->GetTMController()->GetPositionController();
								if (posControl)
								{
									SetControllerKeys(
										posControl,
										&xans.XANK->at(k),
										KEY_POS,
										xans.XANK->size(), k);
								}
							}
						}
					}
				}
				ResumeAnimate();
			}
		}

	}
	return nameAnim;
}

INode* DrawPart(RIFF* riff, SPART* part, int LOD, int PartNo, std::wstring nameAnim, int nodeIndex, std::wstring LODStr)
{
	
	NodeIndex++;
	INode *node;
	{
		TriObject *object = CreateNewTriObject();
		if (!object)
			return FALSE;
		Mesh* mesh = &object->GetMesh();
		int CurrVerb = part->vertex_buffer_index;
		std::vector<SVERT>* vert = riff->VERB->at(CurrVerb).VERT;
		std::vector<SIND3>* inde = riff->IND3;

		std::vector<Point3>* tempVert = new std::vector<Point3>();
		std::vector<Point3>* tempVertN = new std::vector<Point3>();
		Point3 tp;
		Point3 tpn;
		std::vector<int>* tempInd = new std::vector<int>(part->vertex_count);
		int fi = 0;
		for (int ii = 0; ii < part->vertex_count; ii++)
		{
			bool find = false;
			tp = {
				vert->at(ii + part->vertex_offset).position_x,
				vert->at(ii + part->vertex_offset).position_y,
				-vert->at(ii + part->vertex_offset).position_z
			};
			tpn = {
				vert->at(ii + part->vertex_offset).normal_x,
				vert->at(ii + part->vertex_offset).normal_y,
				-vert->at(ii + part->vertex_offset).normal_z
			};
			int iii = 0;
			for (iii = 0; iii < tempVert->size(); iii++)
			{
				if (tp == tempVert->at(iii))
				{
					if (tpn == tempVertN->at(iii))
					{
						find = true;
						break;
					}
				}
			}
			if (!find)
			{
				tempVert->push_back(tp);
				tempVertN->push_back(tpn);
				tempInd->at(ii) = fi;

				fi++;
			}
			else
			{
				tempInd->at(ii) = iii;
			}
		}
		std::vector<Point3>* tempTVert = new std::vector<Point3>();
		std::vector<int>* tempTInd = new std::vector<int>(part->vertex_count);
		fi = 0;
		for (int ii = 0; ii < part->vertex_count; ii++)
		{
			bool find = false;
			tp = {
				vert->at(ii + part->vertex_offset).texture_mapping_x,
				vert->at(ii + part->vertex_offset).texture_mapping_y,
				float(0.0)
			};
			int iii = 0;
			for (iii = 0; iii < tempTVert->size(); iii++)
			{
				if (tp == tempTVert->at(iii))
				{
					find = true;
					break;
				}
			}
			if (!find)
			{
				tempTVert->push_back(tp);
				tempTInd->at(ii) = fi;
				fi++;
			}
			else
			{
				tempTInd->at(ii) = iii;
			}
		}

		std::vector<Point3>* tempNVert = new std::vector<Point3>();
		std::vector<int>* tempNInd = new std::vector<int>(part->vertex_count);
		fi = 0;
		for (int ii = 0; ii < part->vertex_count; ii++)
		{
			bool find = false;
			tp = {
				vert->at(ii + part->vertex_offset).normal_x,
				vert->at(ii + part->vertex_offset).normal_y,
				-vert->at(ii + part->vertex_offset).normal_z
			};
			int iii = 0;
			for (iii = 0; iii < tempNVert->size(); iii++)
			{
				if (tp == tempNVert->at(iii))
				{
					find = true;
					break;
				}
			}
			if (!find)
			{
				tempNVert->push_back(tp);
				tempNInd->at(ii) = fi;
				fi++;
			}
			else
			{
				tempNInd->at(ii) = iii;
			}
		}

		mesh->SpecifyNormals();
		MeshNormalSpec *ns = mesh->GetSpecifiedNormals();
		ns->ClearNormals();
		ns->SetNumNormals(tempNVert->size());
		ns->CheckNormals();

		mesh->setNumVerts(tempVert->size());
		mesh->setNumTVerts(tempTVert->size());

		for (int ii = 0; ii < tempVert->size(); ii++)
		{
			mesh->setVert(ii, tempVert->at(ii));
		}
		for (int ii = 0; ii < tempTVert->size(); ii++)
		{
			mesh->setTVert(ii, tempTVert->at(ii));
		}
		for (int ii = 0; ii < tempNVert->size(); ii++)
		{
			ns->Normal(ii) = tempNVert->at(ii);
			ns->SetNormalExplicit(ii, true);
		}
		mesh->setNumFaces(part->index_count / 3);
		mesh->setNumTVFaces(part->index_count / 3);
		ns->SetNumFaces(part->index_count / 3);
		for (int ii = 0; ii < (part->index_count / 3); ii++)
		{

			mesh->faces[ii].setVerts(
				tempInd->at((inde->at(ii + part->index_offset / 3).ind3)),
				tempInd->at((inde->at(ii + part->index_offset / 3).ind2)),
				tempInd->at((inde->at(ii + part->index_offset / 3).ind1)));
			mesh->faces[ii].setEdgeVisFlags(1, 1, 1);

			mesh->tvFace[ii].setTVerts(
				tempTInd->at((inde->at(ii + part->index_offset / 3).ind3)),
				tempTInd->at((inde->at(ii + part->index_offset / 3).ind2)),
				tempTInd->at((inde->at(ii + part->index_offset / 3).ind1)));

			//RVertex* rv = mesh->getRVertPtr(tempTInd->at((inde->at(ii + part->index_offset / 3).ind1)));
			//rv->rn.addNormal(tempNVert->at(tempNInd->at((inde->at(ii + part->index_offset / 3).ind1))));
			//rv = mesh->getRVertPtr(tempTInd->at((inde->at(ii + part->index_offset / 3).ind2)));
			//rv->rn.addNormal(tempNVert->at(tempNInd->at((inde->at(ii + part->index_offset / 3).ind2))));
			//rv = mesh->getRVertPtr(tempTInd->at((inde->at(ii + part->index_offset / 3).ind3)));
			//rv->rn.addNormal(tempNVert->at(tempNInd->at((inde->at(ii + part->index_offset / 3).ind3))));

			mesh->setNormal(tempTInd->at((inde->at(ii + part->index_offset / 3).ind1)), tempNVert->at(tempNInd->at((inde->at(ii + part->index_offset / 3).ind1))));
			mesh->setNormal(tempTInd->at((inde->at(ii + part->index_offset / 3).ind2)), tempNVert->at(tempNInd->at((inde->at(ii + part->index_offset / 3).ind2))));
			mesh->setNormal(tempTInd->at((inde->at(ii + part->index_offset / 3).ind3)), tempNVert->at(tempNInd->at((inde->at(ii + part->index_offset / 3).ind3))));

			MeshNormalFace &nf = ns->Face(ii);
			nf.SpecifyAll();
			nf.SetNormalID(0, tempNInd->at((inde->at(ii + part->index_offset / 3).ind3)));
			nf.SetNormalID(1, tempNInd->at((inde->at(ii + part->index_offset / 3).ind2)));
			nf.SetNormalID(2, tempNInd->at((inde->at(ii + part->index_offset / 3).ind1)));

		}
		AdjEdgeList* el = new AdjEdgeList(*mesh);
		AdjFaceList * fl = new AdjFaceList(*mesh, *el);
		FaceElementList * fe = new FaceElementList(*mesh, *fl);
		for (int i = 0; i < fe->elem.Count(); i++)
		{
			mesh->faces[i].setSmGroup(fe->elem[i]);
		}
		for (int ii = 0; ii < tempVert->size(); ii++)
		{
			mesh->setNormal(ii, tempVertN->at(ii));
		}

		node = riff->gi->CreateObjectNode(object);
		node->SetMtl(Mtls->at(part->material_index));
		if (!node) {
			delete object;
			return FALSE;
		}
		SParts lNodeParts;
		lNodeParts.LOD = LOD;
		lNodeParts.node = node;
		lNodeParts.Part = PartNo;
		lNodeParts.Inds = tempInd;
		NodeParts->push_back(lNodeParts);

		mesh->buildNormals();
		//mesh->buildBoundingBox();
		//mesh->InvalidateEdgeList();
		mesh->InvalidateGeomCache();
		mesh->InvalidateTopologyCache();
		

	}
	node->SetWireColor(RGB(255, 255, 255));
	//nameAnim = SetAnim(riff, node, nodeIndex);
	if (nameAnim == L"")
	{
		if (riff->SGVL->at(nodeIndex).vis_index >= 0)
		{
			std::wstring NameVis = std::wstring(riff->VISI->at(riff->SGVL->at(nodeIndex).vis_index).name.begin(), riff->VISI->at(riff->SGVL->at(nodeIndex).vis_index).name.end());
			std::wstring XMLVis = StartUD + L"<Visibility name=\"" + NameVis + L"\"></Visibility>" + EndUD;
			node->SetUserPropBuffer(XMLVis.c_str());
			node->SetName(std::wstring(NameVis + L"_" + std::to_wstring(NodeIndex) + L"_" + std::to_wstring(nodeIndex) + L"_P" + LODStr).c_str());
		}
		else
		{
			node->SetName(std::wstring(L"Node_" + std::to_wstring(NodeIndex) + L"_" + std::to_wstring(nodeIndex) + L"_P" +  LODStr).c_str());
		}
	}
	else
	{
		node->SetName(std::wstring(nameAnim + L"_" + std::to_wstring(NodeIndex) + L"_" + std::to_wstring(nodeIndex) + L"_P" +  LODStr).c_str());
	}
	return node;
}

INode* DrawPart2(RIFF* riff, SPART* part, int LOD, int PartNo, std::wstring nameAnim, int nodeIndex)
{

	NodeIndex++;
	INode *node;
	{
		TriObject *object = CreateNewTriObject();
		if (!object)
			return FALSE;
		Mesh* mesh = &object->GetMesh();
		int CurrVerb = part->vertex_buffer_index;
		std::vector<SVERT>* vert = riff->VERB->at(CurrVerb).VERT;
		std::vector<SIND3>* inde = riff->IND3;
		

		//Создаем временный массив для подрезанных вершин
		std::vector<Point3>* tempVert = new std::vector<Point3>();
		//Создаем временный массив для ссылок на реальные вершины
		std::vector<int>* tempInd = new std::vector<int>(part->vertex_count);
		//Подрезаем вершины с одинаковыми координатами
		int fi = 0;
		for (int ii = 0; ii < part->vertex_count; ii++)
		{
			bool find = false;
			Point3 tp = {
			vert->at(ii + part->vertex_offset).position_x,
			vert->at(ii + part->vertex_offset).position_y,
			-vert->at(ii + part->vertex_offset).position_z
			};

			int iii = 0;
			for (iii = 0; iii < tempVert->size(); iii++)
			{
				if (tp == tempVert->at(iii))
				{
					find = true;
					break;
				}
			}
			if (!find)
			{
				tempVert->push_back(tp);
				tempInd->at(ii) = fi;

				fi++;
			}
			else
			{
				tempInd->at(ii) = iii;
			}
		}

		
		//Временный массив для текстурных координат
		std::vector<Point3>* tempTVert = new std::vector<Point3>();
		for (int ii = 0; ii < part->vertex_count; ii++)
		{
			Point3 tp = {
			vert->at(ii + part->vertex_offset).texture_mapping_x,
			vert->at(ii + part->vertex_offset).texture_mapping_y,
			float(0.0)
			};
			tempTVert->push_back(tp);
		}

		//Определяем количество вершин, текстурных координат, треугольников, текстурных треугольников
		mesh->setNumVerts(tempVert->size());
		mesh->setNumTVerts(tempTVert->size());
		mesh->setNumFaces(part->index_count / 3);
		mesh->setNumTVFaces(part->index_count / 3);
		//Строим вершины
		for (int ii = 0; ii < tempVert->size(); ii++)
		{
			mesh->setVert(ii, tempVert->at(ii));
		}
		//Строим текстурные вершины
		for (int ii = 0; ii < tempTVert->size(); ii++)
		{
			mesh->setTVert(ii, tempTVert->at(ii));
		}
		std::vector<Point3>* tempNVert = new std::vector<Point3>(part->vertex_count);
		//std::vector<int>* tempNInd = new std::vector<int>(part->vertex_count);
		fi = 0;
		for (int ii = 0; ii < part->vertex_count; ii++)
		{
	
			Point3 tp = {
			vert->at(ii + part->vertex_offset).normal_x,
			vert->at(ii + part->vertex_offset).normal_y,
			-vert->at(ii + part->vertex_offset).normal_z
			};
			tempNVert->push_back(tp);
		}



		mesh->SpecifyNormals();
		MeshNormalSpec *ns = mesh->GetSpecifiedNormals();
		ns->ClearNormals();
		ns->SetNumNormals(vert->size());
		ns->CheckNormals();
		ns->SetNumFaces(part->index_count / 3);
		

		
		/*for (int ii = 0; ii < tempNVert->size(); ii++)
		{
			ns->Normal(ii) = tempNVert->at(ii);
			ns->SetNormalExplicit(ii, true);
		}*/
		
		//Устанавливаем количество нормалей вершин
		
		for (int ii = 0; ii < (part->index_count / 3); ii++)
		{
			//Устанавливаем треугольники
			mesh->faces[ii].setVerts(
				tempInd->at((inde->at(ii + part->index_offset / 3).ind3)),
				tempInd->at((inde->at(ii + part->index_offset / 3).ind2)),
				tempInd->at((inde->at(ii + part->index_offset / 3).ind1)));
			mesh->faces[ii].setEdgeVisFlags(1, 1, 1);

			//Устанавливаем текстуры треугольников
			mesh->tvFace[ii].setTVerts(
				inde->at(ii + part->index_offset / 3).ind3,
				inde->at(ii + part->index_offset / 3).ind2,
				inde->at(ii + part->index_offset / 3).ind1);

			/*mesh->tvFace[ii].setTVerts(
				tempTInd->at((inde->at(ii + part->index_offset / 3).ind3)),
				tempTInd->at((inde->at(ii + part->index_offset / 3).ind2)),
				tempTInd->at((inde->at(ii + part->index_offset / 3).ind1)));*/

			//RVertex* rv = mesh->getRVertPtr(tempTInd->at((inde->at(ii + part->index_offset / 3).ind1)));
			//rv->rn.addNormal(tempNVert->at(tempNInd->at((inde->at(ii + part->index_offset / 3).ind1))));
			//rv = mesh->getRVertPtr(tempTInd->at((inde->at(ii + part->index_offset / 3).ind2)));
			//rv->rn.addNormal(tempNVert->at(tempNInd->at((inde->at(ii + part->index_offset / 3).ind2))));
			//rv = mesh->getRVertPtr(tempTInd->at((inde->at(ii + part->index_offset / 3).ind3)));
			//rv->rn.addNormal(tempNVert->at(tempNInd->at((inde->at(ii + part->index_offset / 3).ind3))));

			mesh->setNormal(inde->at(ii + part->index_offset / 3).ind1, tempNVert->at(inde->at(ii + part->index_offset / 3).ind1));
			mesh->setNormal(inde->at(ii + part->index_offset / 3).ind2, tempNVert->at(inde->at(ii + part->index_offset / 3).ind2));
			mesh->setNormal(inde->at(ii + part->index_offset / 3).ind3, tempNVert->at(inde->at(ii + part->index_offset / 3).ind3));

			MeshNormalFace &nf = ns->Face(ii);
			nf.SpecifyAll();
			nf.SetNormalID(0, tempInd->at((inde->at(ii + part->index_offset / 3).ind3)));
			nf.SetNormalID(1, tempInd->at((inde->at(ii + part->index_offset / 3).ind2)));
			nf.SetNormalID(2, tempInd->at((inde->at(ii + part->index_offset / 3).ind1)));

		}


		node = riff->gi->CreateObjectNode(object);
		node->SetMtl(Mtls->at(part->material_index));
		if (!node) {
			delete object;
			return FALSE;
		}
		SParts lNodeParts;
		lNodeParts.LOD = LOD;
		lNodeParts.node = node;
		lNodeParts.Part = PartNo;
		lNodeParts.Inds = tempInd;
		NodeParts->push_back(lNodeParts);

		//mesh->buildNormals();
		mesh->buildBoundingBox();
		mesh->InvalidateEdgeList();
		mesh->InvalidateGeomCache();
		//mesh->InvalidateTopologyCache();


	}
	node->SetWireColor(RGB(255, 255, 255));
	nameAnim = SetAnim(riff, node, nodeIndex);
	if (nameAnim == L"")
	{
		if (riff->SGVL->at(nodeIndex).vis_index >= 0)
		{
			std::wstring NameVis = std::wstring(riff->VISI->at(riff->SGVL->at(nodeIndex).vis_index).name.begin(), riff->VISI->at(riff->SGVL->at(nodeIndex).vis_index).name.end());
			std::wstring XMLVis = StartUD + L"<Visibility name=\"" + NameVis + L"\"></Visibility>" + EndUD;
			node->SetUserPropBuffer(XMLVis.c_str());
			node->SetName(std::wstring(NameVis + L"_" + std::to_wstring(NodeIndex) + L"_" + std::to_wstring(nodeIndex) + L"_P").c_str());
		}
		else
		{
			node->SetName(std::wstring(L"Node_" + std::to_wstring(NodeIndex) + L"_" + std::to_wstring(nodeIndex) + L"_P").c_str());
		}
	}
	else
	{
		node->SetName(std::wstring(nameAnim + L"_" + std::to_wstring(NodeIndex) + L"_" + std::to_wstring(nodeIndex) + L"_P").c_str());
	}
	return node;
}
Matrix3 AddTM(STRAN tran, Matrix3 tm)
{
	Matrix3 tm1
	({
		Point3(tran.m00, tran.m01, -tran.m02),
		Point3(tran.m10, tran.m11, -tran.m12),
		Point3(-tran.m20, -tran.m21, tran.m22),
		Point3(tran.m30, tran.m31, -tran.m32)
	});
	return tm * tm1;

}
Matrix3 CalcTransform(RIFF* riff, int StartIndex, int useTMO = 0)
{
	
	SAMAP amap = riff->AMAP->at(riff->SCEN->at(StartIndex).amap_offset / 8);
	STRAN tran = riff->TRAN->at(amap.matrix);
	Matrix3 tm;
	tm.IdentityMatrix();
	if (useTMO == 1)
	{
		tm.RotateX(deg2rad(-90.0));
	}
	else if (useTMO == 2)
	{
		tm.RotateX(deg2rad(90.0));
	}

	tm = AddTM(tran, tm);
	int parent = riff->SCEN->at(StartIndex).parent;
	while (parent != -1)
	{
		SAMAP amap = riff->AMAP->at(riff->SCEN->at(parent).amap_offset / 8);
		STRAN tran = riff->TRAN->at(amap.matrix);
		tm = AddTM(tran, tm);
		parent = riff->SCEN->at(parent).parent;
	};
	
	tm = tm * tmO;

	return tm;
}

INode* rootNode = NULL;

std::wstring EffectPath = L"C:\\GAMES\\LM\\P3Dv4\\Effects\\";
std::string EffectPathA = "C:\\GAMES\\LM\\P3Dv4\\Effects\\";


std::vector<float> GetArrFromString(LPSTR str)
{
	std::vector<float> a;
	for (LPSTR p = strtok(str, ","); p; p = strtok(NULL, ",")) {
		a.push_back(atof(p));
	}
	return a;
}

INode* CreateBone(RIFF* riff, int CurrNode, int ParentNodeIndex, INode* ParentBone)
{
	INode* node;
	Object*  object;
	//Object* object;
	object = (Object*)riff->gi->CreateInstance(GEOMOBJECT_CLASS_ID, BONE_OBJ_CLASSID);
	node = riff->gi->CreateObjectNode(object);
	node->SetBoneNodeOnOff(TRUE, 0);
	node->ShowBone(2);
	node->SetRenderable(FALSE);
	//object->GetParamBlock()->SetValue()
	float LengthParent = 0.5;
	if (riff->SGBR->at(ParentNodeIndex).bone_index >= 0)
	{
		if ((riff->SGBR->at(ParentNodeIndex).bone_index == 0) ||
			(riff->SGBR->at(ParentNodeIndex).bone_index == 1))
		{
			int m = 0;
		}

		SAMAP amap = riff->AMAP->at(riff->SCEN->at(CurrNode).amap_offset / 8);
		STRAN tran = riff->TRAN->at(amap.matrix);
		Matrix3 tmt;
		tmt.IdentityMatrix();
		tmt = AddTM(tran, tmt);
		Point3 VecLength = tmt.GetTrans();
		LengthParent = VecLength.FLength();
		Interval* interval = new Interval(0, 0);
		float* LengthC = new float(0.0);
		MaxSDK::AssetManagement::AssetUser* au = new MaxSDK::AssetManagement::AssetUser();
		ParentBone->GetObjectRef()->GetParamBlock(boneobj_params)->GetValue(boneobj_length, TimeValue(0), *LengthC, *interval);
		if (*LengthC < LengthParent)
		{
			ParentBone->GetObjectRef()->GetParamBlock(boneobj_params)->SetValue(boneobj_width, 0, LengthParent / 2);
			ParentBone->GetObjectRef()->GetParamBlock(boneobj_params)->SetValue(boneobj_height, 0, LengthParent / 2);
			ParentBone->GetObjectRef()->GetParamBlock(boneobj_params)->SetValue(boneobj_length, 0, LengthParent);
		}
		delete LengthC;
	}
	object->GetParamBlock(boneobj_params)->SetValue(boneobj_width, 0, LengthParent / 2);
	object->GetParamBlock(boneobj_params)->SetValue(boneobj_height, 0, LengthParent / 2);
	object->GetParamBlock(boneobj_params)->SetValue(boneobj_length, 0, LengthParent);

	while (Bones->size() <= riff->SGBR->at(CurrNode).bone_index)
	{
		Bones->push_back(NULL);
	}
	Bones->at(riff->SGBR->at(CurrNode).bone_index) = node;
	if (!node) {
		delete object;
		return FALSE;
	}
	return node;
	
}
int CheckParts(RIFF* riff, int CurrNode)
{
	int count = 0;
	for (int j = 0; j < riff->LODT->size(); j++)
	{
		for (int i = 0; i < riff->LODT->at(j).PART->size(); i++)
		{
			SPART *part = &riff->LODT->at(j).PART->at(i);

			if (part->type == 1)
			{
				if (part->scenegraph_reference == CurrNode)
				{

					count++;
				}
			}
		}
	}
	return count;
}

BOOL CreateNode(RIFF* riff, std::vector<SSCEN>* scenes, int CurrNode, int ParentNodeIndex, INode * ParentNode, INode * ParentBone, INode* ParentPart, int LOD, std::wstring nameAnim = L"")
{

	INode* LastPartNode;

	std::vector<INode*> *Nodes = new std::vector<INode*>();
	enum
	{
		BONE_WIDTH,
		BONE_HEIGHT,
		BONE_TAPER,
		BONE_LENGTH,
	};
	if (CurrNode == -15)
	{
		INode* node;
		Object* object;
		object = new  DummyObject();
		node = riff->gi->CreateObjectNode(object);
		if (!node) {
			delete object;
			return FALSE;
		}
		if (ParentNode != NULL)
		{
			ParentNode->AttachChild(node);
		}
		nameAnim = SetAnim(riff, node, CurrNode);
		node->SetNodeTM(0, CalcTransform(riff, CurrNode));
		OneNode = node;
		ParentNode = node;
		OneNode->SetName(std::wstring(L"Node_" + std::to_wstring(NodeIndex) + L"_" + std::to_wstring(CurrNode) + L"_D").c_str());
	}

	while (CurrNode >= 0)
	{
		Nodes->clear();
		OneNode = NULL;
		LastPartNode = NULL;
		int countParts = 0;
		//////////////////////////

		std::wstring LODStr = L"";
		//for (int j = LOD; j < riff->LODT->size(); j++)
		//{
		LODStr = std::wstring(L"_" + std::to_wstring(riff->LODT->at(LOD).LOD));

		for (int i = 0; i < riff->LODT->at(LOD).PART->size(); i++)
		{
			SPART *part = &riff->LODT->at(LOD).PART->at(i);

			if (part->type == 1)
			{
				if (part->scenegraph_reference == CurrNode)
				{



					countParts++;

					INode* NodePart = DrawPart(riff, part, LOD, i, nameAnim, CurrNode, LODStr);
					Nodes->push_back(NodePart);
					//nameAnim = SetAnim(riff, NodePart, CurrNode);
					//NodePart->SetNodeTM(0, CalcTransform(riff, CurrNode));
					if (riff->SGVL->at(CurrNode).vis_index >= 0)
					{
						std::wstring NameVis = std::wstring(riff->VISI->at(riff->SGVL->at(CurrNode).vis_index).name.begin(), riff->VISI->at(riff->SGVL->at(CurrNode).vis_index).name.end());
						std::wstring XMLVis = StartUD + L"<Visibility name=\"" + NameVis + L"\"></Visibility>" + EndUD;
						NodePart->SetUserPropBuffer(XMLVis.c_str());
						if (nameAnim == L"")
						{
							nameAnim = NameVis;
						}
						NodePart->SetName(std::wstring(NameVis + L"_" + std::to_wstring(NodeIndex) + L"_" + std::to_wstring(CurrNode) + L"_P" + LODStr).c_str());
					}
					if (ParentNode != NULL)
					{
						ParentNode->AttachChild(NodePart);
					}
					OneNode = NodePart;
					LastPartNode = NodePart;
				}
			}
		}
		//}
		int attoi = -1;
		for (int ap = 0; ap < riff->REFP->size(); ap++)
		{

			if (riff->REFP->at(ap).scenegraph_reference == CurrNode)
			{
				NodeIndex++;
				countParts++;
				attoi = 0;
				for (attoi = 0; attoi < riff->ATTO->size(); attoi++)
				{
					if (!std::strcmp(riff->ATTO->at(attoi).attachPoint.c_str(), riff->REFP->at(ap).name.c_str()))
					{
						break;
					}
				}
				NodeIndex++;
				INode *node;
				std::wstring NameFX = std::wstring(riff->ATTO->at(attoi).fxName.begin(), riff->ATTO->at(attoi).fxName.end());
				std::wstring EffectFile = EffectPath + NameFX + L".fx";
				std::string EffectFileA = EffectPathA + riff->ATTO->at(attoi).fxName + ".fx";
				if (GetPrivateProfileInt(L"Particle.0", L"Type", 0, EffectFile.c_str()) == 28)
				{
					LPSTR scaleS = new char[64];
					GetPrivateProfileStringA("Particle.0", "X Scale", "1.0 ,1.0", scaleS, 64, EffectFileA.c_str());
					std::vector<float> scalesX = GetArrFromString(scaleS);
					GetPrivateProfileStringA("ParticleAttributes.0", "Color End", "255, 255, 255, 80", scaleS, 64, EffectFileA.c_str());
					std::vector<float> ColorI = GetArrFromString(scaleS);
					GetPrivateProfileStringA("ParticleAttributes.0", "Inner Cone Angle", "1.0", scaleS, 64, EffectFileA.c_str());
					std::vector<float> ICA = GetArrFromString(scaleS);
					GetPrivateProfileStringA("ParticleAttributes.0", "Outer Cone Angle", "45.0", scaleS, 64, EffectFileA.c_str());
					std::vector<float> OCA = GetArrFromString(scaleS);

					GenLight *gl = (GenLight *)CreateInstance(LIGHT_CLASS_ID, Class_ID(SPOT_LIGHT_CLASS_ID, 0));
					gl->SetHotspot(0, ICA[0]);
					gl->SetFallsize(0, OCA[0]);
					gl->SetRGBColor(0, Point3(ColorI[0] / 255, ColorI[1] / 255, ColorI[2] / 255));
					gl->SetIntensity(0, ColorI[3] * scalesX[0] / 1000.0);
					gl->SetConeDisplay(TRUE);
					gl->SetShadow(FALSE);
					gl->SetUseLight(1);

					INode *nodegl = riff->gi->CreateObjectNode(gl);
					//nodegl->SetNodeTM(0, CalcTransform(riff, CurrNode, 0));
					nodegl->SetWireColor(RGB(ColorI[0], ColorI[1], ColorI[2]));

					GenSphere *gs = (GenSphere *)CreateInstance(GEOMOBJECT_CLASS_ID, Class_ID(SPHERE_CLASS_ID, 0));
					gs->SetParams(0.0001, 4, TRUE, FALSE, 0.0, FALSE, FALSE);
					node = riff->gi->CreateObjectNode(gs);
					node->SetRenderable(FALSE);

					//nameAnim = SetAnim(riff, node, CurrNode);
					//node->SetNodeTM(0, CalcTransform(riff, CurrNode, 1));
					Matrix3 tm;
					tm.IdentityMatrix();
					tm.RotateX(deg2rad(90.0));
					nodegl->SetNodeTM(0, tm);
					node->AttachChild(nodegl);
				}
				else
				{
					GenSphere *gs = (GenSphere *)CreateInstance(GEOMOBJECT_CLASS_ID, Class_ID(SPHERE_CLASS_ID, 0));
					gs->SetParams(0.5, 4, TRUE, FALSE, 0.0, FALSE, FALSE);
					node = riff->gi->CreateObjectNode(gs);
					node->SetRenderable(FALSE);
					//nameAnim = SetAnim(riff, node, CurrNode);
					//Matrix3 tm;
					//tm.IdentityMatrix();
					//tm.RotateX(deg2rad(-90.0));
					//node->SetNodeTM(0, tm);
					//node->SetNodeTM(0, CalcTransform(riff, CurrNode, 1));
					//tm.RotateX(deg2rad(-90.0));
				}

				if (ParentNode != NULL)
				{
					ParentNode->AttachChild(node);
				}
				if (ParentPart != NULL)
				{
					ParentPart->AttachChild(node);
				}
				Nodes->push_back(node);

				
				//node->SetNodeTM(0, CalcTransform(riff, CurrNode, 0));

				node->SetWireColor(RGB(246, 7, 255));
				OneNode = node;
				std::wstring NameAtach = std::wstring(riff->ATTO->at(attoi).attachPoint.begin(), riff->ATTO->at(attoi).attachPoint.end()) + LODStr;
				std::wstring ParamAttach = std::wstring(riff->ATTO->at(attoi).params.begin(), riff->ATTO->at(attoi).params.end());
				//node->SetName(std::wstring(NameAtach + L"_" + std::to_wstring(NodeIndex) +L"_"+ std::to_wstring(ParentNodeIndex) +L"_A").c_str());
				node->SetName(NameAtach.c_str());
				if (attoi != riff->ATTO->size())
				{
					std::wstring XMLAttach = StartUD + L"<Attachpoint name = \"" + NameAtach + L"\"> <AttachedObject> <Effect effectName=\"" + NameFX + L"\" effectParams=\"" + ParamAttach + L"\"/> </AttachedObject> </Attachpoint>" + EndUD;
					node->SetUserPropBuffer(XMLAttach.c_str());
				}
			}
		}

		bool FlagPart = true;
		Matrix3 tmP;
		INode *node;
		BoneNode = NULL;
		if (riff->SGBR->at(CurrNode).bone_index >= 0)
		{
			NodeIndex++;
			node = CreateBone(riff, CurrNode, ParentNodeIndex, ParentBone);
			nameAnim = SetAnim(riff, node, CurrNode);
			node->SetNodeTM(0, CalcTransform(riff, CurrNode));
			if (ParentBone != NULL)
			{
				ParentBone->AttachChild(node);
			}
			else if (ParentNode != NULL)
			{
				ParentNode->AttachChild(node);
			}
			for (int i = 0; i < Nodes->size(); i++)
			{
				node->AttachChild(Nodes->at(i));
				//nameAnim = SetAnim(riff, Nodes->at(i), CurrNode);
				MSTR str;
				Nodes->at(i)->GetUserPropBuffer(str);
				std::wstring sstr = std::wstring(str);
				int find = sstr.find(L"Attachpoint");
				if(find>0)
				{
					Nodes->at(i)->SetNodeTM(0, CalcTransform(riff, CurrNode,1));
				}
				else
				{
					Nodes->at(i)->SetNodeTM(0, CalcTransform(riff, CurrNode, 0));
				}
			}
			node->SetWireColor(RGB(255, 255, 255));
			if (riff->SGBN->at(CurrNode).bone_name == "")
			{
				node->SetName(std::wstring(L"Bone_NN_" + std::to_wstring(NodeIndex) + L"_" + std::to_wstring(CurrNode)).c_str());
			}
			else
			{
				std::wstring BoneName = std::wstring(riff->SGBN->at(CurrNode).bone_name.begin(), riff->SGBN->at(CurrNode).bone_name.end());
				node->SetName(std::wstring(BoneName).c_str());// +L"_" + std::to_wstring(CurrNode)).c_str());
			}
			OneNode = node;
			BoneNode = node;
		}		
		else if (countParts != 1)
		{
				NodeIndex++;
				Object* object;
				object = new  DummyObject();
				node = riff->gi->CreateObjectNode(object);
				if (!node) {
					delete object;
					return FALSE;
				}

				if (ParentNode != NULL)
				{
					ParentNode->AttachChild(node);
				}
				nameAnim = SetAnim(riff, node, CurrNode);
				node->SetNodeTM(0, CalcTransform(riff, CurrNode));
				for (int i = 0; i < Nodes->size(); i++)
				{

					//nameAnim = SetAnim(riff, Nodes->at(i), CurrNode);
					node->AttachChild(Nodes->at(i));
					MSTR str;
					Nodes->at(i)->GetUserPropBuffer(str);
					std::wstring sstr = std::wstring(str);
					int find = sstr.find(L"Attachpoint");
					if (find>0)
					{
						Nodes->at(i)->SetNodeTM(0, CalcTransform(riff, CurrNode, 1));
					}
					else
					{
						Nodes->at(i)->SetNodeTM(0, CalcTransform(riff, CurrNode, 0));
					}
				}
				OneNode = node;
				if (riff->SGVL->at(CurrNode).vis_index >= 0)
				{
					std::wstring NameVis = std::wstring(riff->VISI->at(riff->SGVL->at(CurrNode).vis_index).name.begin(), riff->VISI->at(riff->SGVL->at(CurrNode).vis_index).name.end());
					std::wstring XMLVis = StartUD + L"<Visibility name=\"" + NameVis + L"\"></Visibility>" + EndUD;
					OneNode->SetUserPropBuffer(XMLVis.c_str());
					if (nameAnim == L"")
					{
						nameAnim = NameVis;
					}
					OneNode->SetName(std::wstring(NameVis + L"_" + std::to_wstring(NodeIndex) + L"_" + std::to_wstring(CurrNode) + L"_D" + LODStr).c_str());
				}
				if (nameAnim == L"")
				{
					OneNode->SetName(std::wstring(L"Node_" + std::to_wstring(NodeIndex) + L"_" + std::to_wstring(CurrNode) + L"_D" + LODStr).c_str());
				}
				else
				{
					OneNode->SetName(std::wstring(nameAnim + L"_" + std::to_wstring(NodeIndex) + L"_" + std::to_wstring(CurrNode) + L"_D" + LODStr).c_str());
				}
				if (riff->SGBR->at(CurrNode).bone_index >= 0)
				{

			}
		}
		else
		{
			nameAnim = SetAnim(riff, OneNode, CurrNode);
			MSTR str;
			OneNode->GetUserPropBuffer(str);
			std::wstring sstr = std::wstring(str);
			int find = sstr.find(L"Attachpoint");
			if (find>0)
			{
				OneNode->SetNodeTM(0, CalcTransform(riff, CurrNode, 1));
			}
			else
			{
				OneNode->SetNodeTM(0, CalcTransform(riff, CurrNode, 0));
			}
			//OneNode->SetNodeTM(0, CalcTransform(riff, CurrNode));
		}



		if (NodeIndex == 0)
		{
			rootNode = OneNode;
		}		
		CreateNode(riff, scenes, scenes->at(CurrNode).child_node_index, CurrNode, OneNode, BoneNode, LastPartNode, LOD, nameAnim);		
		CurrNode = scenes->at(CurrNode).peer_node_index;		
		}	
	


	//ParentNode->SetNodeTM(0, tmO);


	/*if (countParts == 1)
	{

		ParentNode = PartNode;
		OneNode = PartNode;
	}*/



	



	//rootNode->SetNodeTM(0, tmR);
	//rootNode->ResetPivot(0);
	//rootNode->ResetTransform(0, TRUE);
	return true;

}

enum NodeType
{
	DUMMY,
	BONE,
	ATTACH,
	PART

};
struct SNode
{
	NodeType Type;
	std::vector<int>* parts;
};
struct SHierarchy
{
	std::vector<SNode>* Nodes;
};
std::vector<SHierarchy>* Hierarchy;

void CreateHierarchy(RIFF* riff, int CurrNode)
{
	if (Hierarchy == NULL)
	{
		Hierarchy = new std::vector<SHierarchy>();
	}
	while (CurrNode >= 0)
	{
		int count = 0;
		for (int j = 0; j < riff->LODT->size(); j++)
		{
			for (int i = 0; i < riff->LODT->at(j).PART->size(); i++)
			{
				SPART *part = &riff->LODT->at(j).PART->at(i);

				if (part->type == 1)
				{
					if (part->scenegraph_reference == CurrNode)
					{
						count++;

					}
				}
			}
		}
		CreateHierarchy(riff, riff->SCEN->at(CurrNode).child_node_index);
		CurrNode = riff->SCEN->at(CurrNode).peer_node_index;
	}
}

BOOL SetSkin(RIFF* riff)
{
	for (int i = 0; i < NodeParts->size(); i++)
	{
		int CurrVerb = riff->LODT->at(NodeParts->at(i).LOD).PART->at(NodeParts->at(i).Part).vertex_buffer_index;
		if (riff->VERB->at(CurrVerb).SKIN != NULL)
		{
			//ISkinImportData *skin = (ISkinImportData*)riff->gi->CreateInstance(OSM_CLASS_ID, Class_ID(9815843, 87654));

			Modifier* skin = (Modifier*)riff->gi->CreateInstance(SClass_ID(OSM_CLASS_ID), SKIN_CLASSID);
			ISkinImportData* SkinInterface = (ISkinImportData*)skin->GetInterface(I_SKINIMPORTDATA);
			
			
			
			//Modifier* mod = (Modifier*)skin;
			GetCOREInterface12()->AddModifier(*NodeParts->at(i).node, *skin);
			//riff->i->AddModifier(*NodeParts->at(i).node, *skin);
			for (int bi = 0; bi < riff->VERB->at(CurrVerb).BMAP->size(); bi++)
			{
				SkinInterface->AddBoneEx(Bones->at(riff->VERB->at(CurrVerb).BMAP->at(bi).index), true);
			}
			int StartVert = riff->LODT->at(NodeParts->at(i).LOD).PART->at(NodeParts->at(i).Part).vertex_offset;
			int CountVert = riff->LODT->at(NodeParts->at(i).LOD).PART->at(NodeParts->at(i).Part).vertex_count;
			GetCOREInterface()->ForceCompleteRedraw();
			NodeParts->at(i).node->EvalWorldState(GetCOREInterface()->GetTime());
			for (int si = StartVert; si < (CountVert+StartVert); si++)
			{
				//std::vector<INode*>* lbones = new std::vector<INode*>();
				//std::vector<float>* lweights = new std::vector<float>();
				Tab<INode*> lbones;
				Tab<float> lweights;
				lbones.SetCount(0);
				lweights.SetCount(0);
				for (int sii = 0; sii < 4; sii++)
				{
					if (riff->VERB->at(CurrVerb).SKIN->at(si).weight[sii] != 0)
					{
						float w = riff->VERB->at(CurrVerb).SKIN->at(si).weight[sii] / 255.0;
						lbones.Append(1, &Bones->at(riff->VERB->at(CurrVerb).BMAP->at(riff->VERB->at(CurrVerb).SKIN->at(si).bn[sii]).index));
						lweights.Append(1, &w);
					}
				}
				if ((si-StartVert) == 71)
				{
					int m = 0;
				}
				bool hr = SkinInterface->AddWeights(NodeParts->at(i).node, NodeParts->at(i).Inds->at(si-StartVert), lbones, lweights);
				if (hr)
				{
					int m = 0;
				}

			}
		}
	}
	return TRUE;
}

BOOL objFileRead(const TCHAR *filename, Mesh *mesh)
{
	float size, h;

	h = 0.5 * 5;
	
	// Число вершин и граней в меш
	mesh->setNumVerts(8);
	mesh->setNumFaces(12);
	// Координаты вершин
	mesh->setVert(0, Point3(-h, -h, -h));
	mesh->setVert(1, Point3(h, -h, -h));
	mesh->setVert(2, Point3(-h, h, -h));
	mesh->setVert(3, Point3(h, h, -h));
	mesh->setVert(4, Point3(-h, -h, h));
	mesh->setVert(5, Point3(h, -h, h));
	mesh->setVert(6, Point3(-h, h, h));
	mesh->setVert(7, Point3(h, h, h));
	// Состав граней
	mesh->faces[0].setVerts(0, 2, 3);
	mesh->faces[1].setVerts(3, 1, 0);
	mesh->faces[2].setVerts(4, 5, 7);
	mesh->faces[3].setVerts(7, 6, 4);
	mesh->faces[4].setVerts(0, 1, 5);
	mesh->faces[5].setVerts(5, 4, 0);
	mesh->faces[6].setVerts(1, 3, 7);
	mesh->faces[7].setVerts(7, 5, 1);
	mesh->faces[8].setVerts(3, 2, 6);
	mesh->faces[9].setVerts(6, 7, 3);
	mesh->faces[10].setVerts(2, 0, 4);
	mesh->faces[11].setVerts(4, 6, 2);
	// Группы сглаживания
	mesh->faces[0].setSmGroup(2);
	mesh->faces[1].setSmGroup(2);
	mesh->faces[2].setSmGroup(4);
	mesh->faces[3].setSmGroup(4);
	mesh->faces[4].setSmGroup(8);
	mesh->faces[5].setSmGroup(8);
	mesh->faces[6].setSmGroup(16);
	mesh->faces[7].setSmGroup(16);
	mesh->faces[8].setSmGroup(32);
	mesh->faces[9].setSmGroup(32);
	mesh->faces[10].setSmGroup(64);
	mesh->faces[11].setSmGroup(64);
	// Устанавливаем видимость внешних ребер граней (диагональные ребра не видны)
	for (int k = 0; k < 12; k++) {
		mesh->faces[k].setEdgeVisFlags(1, 1, 0);
		mesh->faces[k].setMatID(1);
	}
	// Назначаем UVW-координаты
	Matrix3 tm(1);
	tm.Scale(Point3(h, h, h));
	tm = Inverse(tm);
	mesh->ApplyUVWMap(MAP_BOX, 1.0f, 1.0f, 1.0f, 0, 0, 0, 0, tm);
	mesh->InvalidateTopologyCache();
	
	return TRUE;
}
bool CreateMtlLib(RIFF* riff, std::vector<SMAT3>* mate, std::vector<SEMT1>* emate, const TCHAR * filename)
{
	std::vector<BitmapTex*>* bt = new std::vector<BitmapTex*>();
	std::wstring wp = std::wstring(filename);
	std::wstring base_dir = wp.substr(0, wp.find_last_of(L"/\\") + 1);
	for (int i = 0; i < riff->TEXT->size(); i++)
	{
		std::string TexName = riff->TEXT->at(i);
		std::wstring widestr = base_dir + std::wstring(TexName.begin(), TexName.end());
		const wchar_t* widecstr = widestr.c_str();
		BitmapTex* bmt = NewDefaultBitmapTex();
		bmt->SetAlphaAsMono(true);
		StdUVGen *uv = bmt->GetUVGen();
		uv->SetVScl(-1.0, 0);
		bmt->SetMapName(widecstr);
		bt->push_back(bmt);
	}
	for (int i = 0; i < mate->size(); i++)
	{
		SMAT3 lMat = mate->at(i);
		P3DMaterial* mtl = NewP3DMat();
		if (emate->size() > 0)
		{
			SEMT1 leMat = emate->at(i);
			mtl->ConvertPrepar3DMaterials(&lMat, &leMat, bt);
		}
		else
		{
			mtl->ConvertPrepar3DMaterials(&lMat, NULL, bt);
		}
	
		
		//Class_ID P3DMat = { 0x7e8caded , 0xb6928000 };
		//Mtl *mtl = (Mtl*)riff->gi->CreateInstance(MATERIAL_CLASS_ID, P3DMat);
		if (riff->TEXT->size() > 0)
		{
			std::string TexName = riff->TEXT->at(lMat.diffuse_texture_index);
			std::wstring widestr = std::wstring(TexName.begin(), TexName.end());
			const wchar_t* widecstr = widestr.c_str();
			mtl->BaseMat->SetName(WStr(widecstr));
		}
		else
		{
			//TO DO: Add name by color
		}
		/*if (lMat.diffuse_texture_index > 0)
		{
			mtl->SetSubTexmap(3, bt->at(lMat.diffuse_texture_index));
			//mtl->SetSubTexmap(6, bt->at(lMat.diffuse_texture_index));
		}
		if (lMat.bumpmap_texture_index > 0)
		{
			mtl->SetSubTexmap(10, bt->at(lMat.bumpmap_texture_index));
		}
		if (lMat.specular_texture_index > 0)
		{
			mtl->SetSubTexmap(4, bt->at(lMat.specular_texture_index));
		}
		if (lMat.emissive_texture_index > 0)
		{
			mtl->SetSubTexmap(7, bt->at(lMat.emissive_texture_index));
		}
		//mtl->SetDiffuse(Color(lMat.diffuse_color_R, lMat.diffuse_color_G, lMat.diffuse_color_B), 0);
		//mtl->SetSpecular(Color(lMat.specular_color_R, lMat.specular_color_G, lMat.specular_color_B), 0);
		
		//mtl->SetProperty(PROPID_USER + 1, mate->at(i).material_flags & 0x200);
		//mtl->SetOpacity*/
		Mtls->push_back(mtl->BaseMat);
	}
	//Class_ID P3DMat = { 0x7e8caded , 0xb6928000 };
	//Mtl *mtl = (Mtl*)riff->gi->CreateInstance(MATERIAL_CLASS_ID, P3DMat);
	//riff->gi->PutMtlToMtlEditor(mtl, 0);
	//mtl->SetName(TSTR(_T("OUT")));

	//mtl->SetDiffuse(Color(0.9, 0.9, 0.1), 0);
	//bool bFresnelDiffuse = true;
	//mtl->SetProperty(PROPID_USER + 1, &bFresnelDiffuse);
	return true;
}

int MDLConverterv2::DoImport(const TCHAR * filename, ImpInterface * i, Interface * gi, BOOL suppressPrompts)
{
	#pragma message(TODO("Implement the actual file import here and"))	
	RIFF * PRIFF = new RIFF(filename, i, gi);
	tmO.RotateX(deg2rad(90.0));
	CreateMtlLib(PRIFF, PRIFF->MAT3, PRIFF->EMT1, filename);

	INode* nodeSR;
	Object* objectSR;
	objectSR = new  DummyObject();
	nodeSR = PRIFF->gi->CreateObjectNode(objectSR);
	//nodeExt->SetName((std::wstring(PRIFF->MDLN.begin(), PRIFF->MDLN.end()) + L"_Exterior").c_str());
	nodeSR->SetName(L"Scene Root");
	if (!nodeSR) {
		delete objectSR;
		return FALSE;
	}
	INode* nodeExt;
	Object* objectExt;
	objectExt = new  DummyObject();
	nodeExt = PRIFF->gi->CreateObjectNode(objectExt);
	nodeExt->SetName((std::wstring(PRIFF->MDLN.begin(), PRIFF->MDLN.end()) + L"_Exterior").c_str());
	//nodeExt->SetName(L"Scene Root");
	nodeSR->AttachChild(nodeExt);
	if (!nodeExt) {
		delete objectExt;
		return FALSE;
	}
	for (int j = 0; j < PRIFF->LODT->size(); j++)
	{
		INode* nodeLOD;
		Object* objectLOD;
		objectLOD = new  DummyObject();
		nodeLOD = PRIFF->gi->CreateObjectNode(objectLOD);
		nodeLOD->SetName((std::wstring(PRIFF->MDLN.begin(), PRIFF->MDLN.end()) + L"_LOD_" + std::to_wstring(PRIFF->LODT->at(j).LOD)).c_str());
		//nodeExt->SetName(L"Scene Root");
		nodeExt->AttachChild(nodeLOD);
		if (!nodeLOD) {
			delete objectLOD;
			return FALSE;
		}
		//LODStr = std::wstring(L"_LOD_" + std::to_wstring(riff->LODT->at(LOD).LOD));
		CreateNode(PRIFF, PRIFF->SCEN, 0, 0, nodeLOD, NULL, NULL, j);
	}
	SetSkin(PRIFF);
	/*for (int i = 0; i < PRIFF->LODT->at(0).PART->size(); i++)
	{
		DrawPart(PRIFF, &PRIFF->LODT->at(0).PART->at(i));
	}*/
	/*for (int i = 0; i < 450; i++)
	{
		DrawPart(PRIFF, &PRIFF->LODT->at(0).PART->at(i));
	}*/
	/*for (int i = 0; i < PRIFF->LODT->at(0).PART->size(); i++)
	{
		if (i == 228)
		{
			int i = 0;
			//return TRUE;
		}
		short parent = PRIFF->SCEN->at(PRIFF->LODT->at(0).PART->at(i).scenegraph_reference).parent;
		if (parent != -1)
		{
			if (PRIFF->SCEN->at(parent).parentNode == NULL)
			{
				
				TriObject *object = CreateNewTriObject();
				INode *node2 = PRIFF->gi->CreateObjectNode(object);
				if (!node2) {
					delete object;
					return FALSE;
				}
				PRIFF->SCEN->at(parent).parentNode = node2;
				node2->SetName(L"Dummy");
			}
			PRIFF->SCEN->at(parent).parentNode->AttachChild(
				PRIFF->SCEN->at(PRIFF->LODT->at(0).PART->at(i).scenegraph_reference).parentNode
			);
		}
	}*/
	delete PRIFF;
	PRIFF = NULL;
	TriObject *object = CreateNewTriObject();
	if (!object)
		return 0;

	/*if (objFileRead(filename, &object->GetMesh())) {
		INode *node = gi->CreateObjectNode(object);
		if (!node) {
			delete object;
			return FALSE;
		}
		Matrix3 tm;
		tm.IdentityMatrix();
		//node->Reference(object);
		node->SetNodeTM(0, tm);
		//gi->AddNodeToScene(node);
		node->SetName(L"TestMesh");
		Class_ID P3DMat = { 0x7e8caded , 0xb6928000 };
		Mtl *mtlb = (Mtl*)gi->CreateInstance(MATERIAL_CLASS_ID, P3DMat);
		gi->PutMtlToMtlEditor(mtlb, 0);
		INode* node1 = node;
		mtlb->SetName(TSTR(_T("OUT")));
		node->SetMtl(mtlb);
		mtlb->SetDiffuse(Color(0.9, 0.9, 0.1), 0);
		bool bFresnelDiffuse = true;
		mtlb->SetProperty(PROPID_USER + 1, &bFresnelDiffuse);
		bFresnelDiffuse = mtlb->GetProperty(PROPID_USER + 1);
		int PBN = mtlb->NumParamBlocks();
		IParamBlock2 *PB = mtlb->GetParamBlock(0);
		ParamBlockDesc2* desc = PB->GetDesc();
		int NP = desc->Count();
		ParamDef mc;
		PB->SetValue(0, 0, true, 0);
		//PB->SetDesc(desc);
		int PID = desc->NameToIndex(L"bFresnelDiffuse");
		for (int ii = 0; ii < NP; ii++)
		{

			mc = desc->paramdefs[ii];
		}
		i->RedrawViews();
		//ParamAlias *PA = PB->FindParamAlias(TSTR(_T("bFresnelDiffuse")));
		//ParamAlias *PA2 = PB->FindParamAlias(TSTR(_T("ui:bDiffuse")));
		//mtlb->SetProperty(PA->ID, &bFresnelDiffuse);
		//mtlb->SetProperty();1
		/*gi->LoadDefaultMatLib();
		MtlBaseLib &ml = *gi->GetSceneMtls();



		//int mli = ml->FindMtlByName(TSTR(_T("Prepar3D")));
		int count = ml.Count();
		//int index = mlib1.FindMtlByName(TSTR(_T("Prepar3D")));
		if (count <= 0)
		{
		return FALSE;
		}
		//Mtl *m1 = new Mtl();
		MtlBase *m2 = ml[0];
		Class_ID class1 = m2->ClassID();

		//m->ActivateTexDisplay(FALSE);

		if (gi->GetSelNodeCount() < 1)
		{
		return TRUE;
		}
		//INode *node = gi->GetSelNode(0);*
		
		return TRUE;
	}*/
	return TRUE;
	/*if(!suppressPrompts)
		DialogBoxParam(hInstance, 
				MAKEINTRESOURCE(IDD_PANEL), 
				GetActiveWindow(), 
				MDLConverterv2OptionsDlgProc, (LPARAM)this);

	#pragma message(TODO("return TRUE If the file is imported properly"))
	return FALSE;*/
}
	
