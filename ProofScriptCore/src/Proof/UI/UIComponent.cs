using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    public abstract class UIComponent
    {
        public Entity Entity { get; internal set; }
        public string Name { get; internal set; }
        public uint TableIndex { get; internal set; }

        
    }

    struct UIBaseData
    {
        public Vector2 Position;
        public Vector2 Rotation;
        public Vector2 Size;
        public bool Visible;
        public Vector4 Color;
    }
    public class UIButton : UIComponent
    {
        UIBaseData ButtonData
        {
            get
            {
                return InternalCalls.PlayerHUDComponent_GetButtonData(Entity.ID, TableIndex, Name);
            }
            set
            {
                InternalCalls.PlayerHUDComponent_SetButtonData(Entity.ID, TableIndex, Name, ref value);
            }
        }
        public bool Visible
        {
            get
            {
                return ButtonData.Visible;
            }
            set
            {
                UIBaseData copy = ButtonData;
                copy.Visible = value;
                ButtonData = copy;
            }
        }
        public Vector2 Position
        {
            get
            {
                return ButtonData.Position;
            }
            set
            {
                UIBaseData copy = ButtonData;
                copy.Position = value;
                ButtonData = copy;
            }
        }
        public Vector2 Rotation
        {
            get { return ButtonData.Rotation; }
            set
            {
                UIBaseData copy = ButtonData;
                copy.Rotation = value;
                ButtonData = copy;
            }

        }
        public Vector2 Size
        {
            get { return ButtonData.Size; }
            set
            {
                UIBaseData copy = ButtonData;
                copy.Size = value;
                ButtonData = copy;
            }
        }
        public Vector4 Color
        {
            get { return ButtonData.Color; }

            set
            {
                UIBaseData copy = ButtonData;
                copy.Color = value;
                ButtonData = copy;
            }
        }
    }

    struct UiImageButtonData
    {
        public UIBaseData BaseData;
        public Texture ImageAssetID;
    }
    struct UITextData
    {
        public UIBaseData BaseData;
        public float Kerning;
        public float LineSpacing;
    }
    public class UIButtonImage : UIComponent
    {
        UiImageButtonData ButtonData
        {
            get
            {
                return InternalCalls.PlayerHUDComponent_GetImageButtonData(Entity.ID, TableIndex, Name);
            }
            set
            {
                InternalCalls.PlayerHUDComponent_SetImageButtonData(Entity.ID, TableIndex, Name,ref value);
            }
        }
        public bool Visible
        {
            get
            {
                return ButtonData.BaseData.Visible;
            }
            set
            {
                UiImageButtonData copy = ButtonData;
                copy.BaseData.Visible = value;
                ButtonData = copy;
            }
        }
        public Vector2 Position
        {
            get
            {
                return ButtonData.BaseData.Position;
            }
            set
            {
                UiImageButtonData copy = ButtonData;
                copy.BaseData.Position = value;
                ButtonData = copy;
            }
        }
        public Vector2 Rotation
        {
            get {return ButtonData.BaseData.Rotation;}
            set
            {
                UiImageButtonData copy = ButtonData;
                copy.BaseData.Rotation = value;
                ButtonData = copy;
            }

        }
        public Vector2 Size
        {
            get { return ButtonData.BaseData.Size; }
            set
            {
                UiImageButtonData copy = ButtonData;
                copy.BaseData.Size = value;
                ButtonData = copy;
            }
        }
        public Vector4 Color
        {
            get { return ButtonData.BaseData.Color; }

            set
            {
                UiImageButtonData copy = ButtonData;
                copy.BaseData.Color = value;
                ButtonData = copy;
            }
        }
        public Texture Image
        {
            get { return ButtonData.ImageAssetID; }

            set
            {
                UiImageButtonData copy = ButtonData;
                copy.ImageAssetID = value;
                ButtonData = copy;
            }
        }
    }

    public class UIText : UIComponent
    {
        UITextData TextData
        {
            get
            {
                InternalCalls.PlayerHUDComponent_GetTextData(Entity.ID, TableIndex, Name, out UITextData data, out string text);
                return data;
            }
            set
            {
                InternalCalls.PlayerHUDComponent_GetTextData(Entity.ID, TableIndex, Name, out UITextData data, out string text);
                InternalCalls.PlayerHUDComponent_SetTextData(Entity.ID, TableIndex, Name, ref value, ref text);
            }
        }

        public string Text
        {
            get
            {
                InternalCalls.PlayerHUDComponent_GetTextData(Entity.ID, TableIndex, Name, out UITextData data, out string text);
                return text;
            }
            set
            {
                InternalCalls.PlayerHUDComponent_GetTextData(Entity.ID, TableIndex, Name, out UITextData data, out string text);
                InternalCalls.PlayerHUDComponent_SetTextData(Entity.ID, TableIndex, Name, ref data, ref value);
            }
        }
        public bool Visible
        {
            get
            {
                return TextData.BaseData.Visible;
            }
            set
            {
                UITextData copy = TextData;
                copy.BaseData.Visible = value;
                TextData = copy;
            }
        }

        public Vector2 Position
        {
            get
            {
                return TextData.BaseData.Position;
            }
            set
            {
                UITextData copy = TextData;
                copy.BaseData.Position = value;
                TextData = copy;
            }
        }
        public Vector2 Rotation
        {
            get { return TextData.BaseData.Rotation; }
            set
            {
                UITextData copy = TextData;
                copy.BaseData.Rotation = value;
                TextData = copy;
            }

        }
        public Vector2 Size
        {
            get { return TextData.BaseData.Size; }
            set
            {
                UITextData copy = TextData;
                copy.BaseData.Size = value;
                TextData = copy;
            }
        }
        public Vector4 Color
        {
            get { return TextData.BaseData.Color; }

            set
            {
                UITextData copy = TextData;
                copy.BaseData.Color = value;
                TextData = copy;
            }
        }
        public float Kerning
        {
            get { return TextData.Kerning; }
            set
            {
                UITextData copy = TextData;
                copy.Kerning = value;
                TextData = copy;
            }
        }
        public float LineSpacing
        {
            get { return TextData.LineSpacing; }
            set
            {
                UITextData copy = TextData;
                copy.LineSpacing = value;
                TextData = copy;
            }
        }
    }

    public class UIPanel
    {
        public uint TableIndex { get; internal set; }
        public Entity Entity { get; internal set; }
        

        internal UIPanel(uint index, Entity entity)
        {
            Entity = entity;
            TableIndex = index;
        }
        public bool Visible
        {
            get { return InternalCalls.PlayerHUDComponent_GetVisible(Entity.ID, TableIndex);  }
            set
            {
                InternalCalls.PlayerHUDComponent_SetVisible(Entity.ID, TableIndex, ref value);
            }
        }
        public UIButton GetUIButton(string name) {
            if (InternalCalls.PlayerHUDComponent_HasButton(Entity.ID, TableIndex, name))
            {
                return new UIButton() { Name = name, Entity = Entity, TableIndex = TableIndex };
            }
            return null;
        }
        public UIButtonImage GetImageButton(string name)
        {

            if (InternalCalls.PlayerHUDComponent_HasImageButton(Entity.ID, TableIndex, name))
            {
                return new UIButtonImage() { Name = name, Entity = Entity, TableIndex = TableIndex };
            }
            return null;
        }

        public UIText GetText(string name) 
        {
            if (InternalCalls.PlayerHUDComponent_HasText(Entity.ID, TableIndex, name))
            {
                return new UIText() { Name = name, Entity = Entity, TableIndex = TableIndex };
            }
            return null;
        }
    }
}
