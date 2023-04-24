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
        public ImageAsset ImageAssetID;
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
        public ImageAsset Image
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
