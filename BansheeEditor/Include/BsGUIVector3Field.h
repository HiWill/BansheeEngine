#pragma once

#include "BsEditorPrerequisites.h"
#include "BsGUIFieldBase.h"
#include "CmVector3.h"

namespace BansheeEditor
{
	class BS_ED_EXPORT GUIVector3Field : public TGUIField<GUIVector3Field>
	{
	public:
		static const CM::String& getGUITypeName();

		GUIVector3Field(const PrivatelyConstruct& dummy, BS::GUIWidget& parent, const BS::GUIContent& labelContent, CM::UINT32 labelWidth,
			BS::GUIElementStyle* labelStyle, BS::GUIElementStyle* inputBoxStyle, const BS::GUILayoutOptions& layoutOptions, bool withLabel);

		CM::Vector3 getValue() const;
		void setValue(const CM::Vector3& value);

	protected:
		virtual ~GUIVector3Field() { }

	protected:
		static const CM::UINT32 ELEMENT_LABEL_WIDTH;

		GUIFloatField* mFieldX;
		GUIFloatField* mFieldY;
		GUIFloatField* mFieldZ;
	};
}