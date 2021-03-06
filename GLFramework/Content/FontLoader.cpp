#include "stdafx.hpp"
#include "FontLoader.hpp"

#include "../Helper/BinaryReader.h"

FontLoader::FontLoader()
{
}

FontLoader::~FontLoader()
{
}

SpriteFont* FontLoader::LoadContent(const std::string& assetFile)
{
	using namespace std;
	cout << "Loading Texture: " << assetFile << " . . . ";

	auto pBinReader = new BinaryReader(); //Prevent memory leaks
	pBinReader->Open(assetFile);

	if (!pBinReader->Exists())
	{
		delete pBinReader;
		std::cout << " . . . FAILED!\n[ERROR] " << "SpriteFont::Load > Failed to read the assetFile!\nPath: " << assetFile << std::endl;

		return nullptr;
	}
	bool valid = false;
	if (pBinReader->Read<char>() == 'B')
	{
		if (pBinReader->Read<char>() == 'M')
		{
			if (pBinReader->Read<char>() == 'F')
			{
				valid = true;
			}
		}
	}
	if (!valid) { std::cout << " . . . FAILED!\n[ERROR] " << "Font file header invalid!" << std::endl; return nullptr; }
	if (pBinReader->Read<char>() < 3)
	{
		std::cout << " . . . FAILED!\n[ERROR] " << "Font version invalid!" << std::endl; return nullptr;
	}

	SpriteFont* pFont = new SpriteFont();

	//**********
	// BLOCK 0 *
	//**********
	pBinReader->Read<char>();
	auto Block0Size = pBinReader->Read<int>();
	int pos = pBinReader->GetBufferPosition();
	pFont->m_FontSize = pBinReader->Read<short>();
	pBinReader->SetBufferPosition(pos + 14);
	std::string fn;
	char cur = pBinReader->Read<char>();
	while (cur != '\0')
	{
		fn += cur;
		cur = pBinReader->Read<char>();
	}
	pFont->m_FontName = fn;
	pBinReader->SetBufferPosition(pos + Block0Size);
	//**********
	// BLOCK 1 *
	//**********
	pBinReader->Read<char>();
	auto Block1Size = pBinReader->Read<int>();
	pos = pBinReader->GetBufferPosition();
	pBinReader->SetBufferPosition(pos + 4);
	pFont->m_TextureWidth = pBinReader->Read<unsigned short>();
	pFont->m_TextureHeight = pBinReader->Read<unsigned short>();
	auto pagecount = pBinReader->Read<unsigned short>();
	if (pagecount > 1)std::cout << "[ERROR] " <<
		"SpriteFont::Load > SpriteFont(.fnt): Only one texture per font allowed" << std::endl;
	pBinReader->SetBufferPosition(pos + Block1Size);
	//**********
	// BLOCK 2 *
	//**********
	pBinReader->Read<char>();
	auto Block2Size = pBinReader->Read<int>();
	pos = pBinReader->GetBufferPosition();
	std::string pn;
	cur = pBinReader->Read<char>();
	while (cur != '\0')
	{
		pn += cur;
		cur = pBinReader->Read<char>();
	}
	if (pn.size() == 0)std::cout << "[ERROR] " <<
		"SpriteFont::Load > SpriteFont(.fnt): Invalid Font Sprite [Empty]" << std::endl;
	auto filepath = assetFile.substr(0, assetFile.rfind('/') + 1);
	std::cout << std::endl << "\t";
	pFont->m_pTexture = ContentManager::Load<TextureData>(filepath + pn);
	pBinReader->SetBufferPosition(pos + Block2Size);
	//**********
	// BLOCK 3 *
	//**********
	pBinReader->Read<char>();
	auto Block3Size = pBinReader->Read<int>();
	pos = pBinReader->GetBufferPosition();
	auto numChars = Block3Size / 20;
	pFont->m_CharacterCount = numChars;
	for (int i = 0; i < numChars; i++)
	{
		auto posChar = pBinReader->GetBufferPosition();
		auto charId = (wchar_t)(pBinReader->Read<unsigned int>());
		if (!(pFont->IsCharValid(charId)))
		{
			std::cout << "[WARNING] " <<
				"SpriteFont::Load > SpriteFont(.fnt): Invalid Character" << std::endl;
			pBinReader->SetBufferPosition(posChar + 20);
		}
		else
		{
			auto metric = &(pFont->GetMetric(charId));
			metric->IsValid = true;
			metric->Character = charId;
			auto xPos = pBinReader->Read<unsigned short>();
			auto yPos = pBinReader->Read<unsigned short>();
			metric->Width = pBinReader->Read<unsigned short>();
			metric->Height = pBinReader->Read<unsigned short>();
			metric->OffsetX = pBinReader->Read<short>();
			metric->OffsetY = pBinReader->Read<short>();
			metric->AdvanceX = pBinReader->Read<short>();
			metric->Page = pBinReader->Read<unsigned char>();
			auto chan = pBinReader->Read<unsigned char>();
			switch (chan)
			{
			case 1: metric->Channel = 2; break;
			case 2: metric->Channel = 1; break;
			case 4: metric->Channel = 0; break;
			case 8: metric->Channel = 3; break;
			default: metric->Channel = 4; break;
			}
			metric->TexCoord = glm::vec2((float)xPos / (float)pFont->m_TextureWidth
				, (float)yPos / (float)pFont->m_TextureHeight);
			pBinReader->SetBufferPosition(posChar + 20);
		}
	}
	delete pBinReader;

	cout << "  . . . SUCCESS!" << endl;

	return pFont;
}

void FontLoader::Destroy(SpriteFont* objToDestroy)
{
	if (!(objToDestroy == nullptr))
	{
		delete objToDestroy;
		objToDestroy = nullptr;
	}
}