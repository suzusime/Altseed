﻿#pragma once

#include "../ace.Core.Base.h"

#include "ace.FileRoot.h"
#include "ace.File.h"
#include "ace.PackFile.h"
#include "ace.FileHelper.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>

namespace std
{
	template<>
	struct hash < shared_ptr <ace::FileRoot> >
	{
		size_t operator()(shared_ptr<ace::FileRoot> ptr) const
		{
			return static_cast<size_t>(hash<ace::astring>()(ptr->m_path));
		}
	};
}

namespace ace
{
	class BaseFile;

	class File_Imp
		: public File
		, public ReferenceObject
	{
	private:
		std::vector<std::shared_ptr<FileRoot>> m_roots;

		//std::unordered_map<astring, StaticFile_Imp*> m_staticFileCash;
		std::unordered_map<astring, StreamFile_Imp*> m_streamFileCash;

		template<typename _T>
		inline bool Valid(_T* ptr) { return ptr && (0 < ptr->GetRef()); }

		
	public:
		static File_Imp* Create() { return new File_Imp(); };

		File_Imp();
		virtual ~File_Imp();
		void AddDefaultRootDirectory();

		virtual void AddRootDirectory(const achar* path);
		virtual void AddRootPackage(const achar* path, const achar* key);
		virtual void ClearRootDirectories();
		virtual bool Exists(const achar* path) const;
		//virtual StreamFile* CreateStreamFile(const achar* path);
		virtual StaticFile* CreateStaticFile_(const achar* path);

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
	};
}