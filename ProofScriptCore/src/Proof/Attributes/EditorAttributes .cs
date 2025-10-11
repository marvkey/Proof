using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    [RegisterCoreClassStruct]
    [AttributeUsage(AttributeTargets.Field | AttributeTargets.Property)]
    public class ShowInEditorAttribute : Attribute
    {
        public string DisplayName = "";
        public bool IsReadOnly = false;

        public ShowInEditorAttribute(string displayName)
        {
            DisplayName = displayName;
        }
        public ShowInEditorAttribute(bool readOnly)
        {
            IsReadOnly = readOnly;
        }
        public ShowInEditorAttribute(string displayName,bool readOnly)
        {
            DisplayName = displayName;
            IsReadOnly = readOnly;
        }

        public ShowInEditorAttribute()
        {

        }

    }

    [RegisterCoreClassStruct]
    [AttributeUsage(AttributeTargets.Field | AttributeTargets.Property)]
    public class ToolTipAttribute : Attribute
    {
        public string ToolTip = "";

        public ToolTipAttribute(string toolTip)
        {
            ToolTip = toolTip;
        }
    }


    [RegisterCoreClassStruct]
    [AttributeUsage(AttributeTargets.Field | AttributeTargets.Property)]
    public class SeperatorAttribute : Attribute
    {
        public SeperatorAttribute()
        {
        }
    }

}
