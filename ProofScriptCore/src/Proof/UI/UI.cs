using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    [StructLayout(LayoutKind.Sequential)]
    public struct UIPanelInstanceRaw
    {
        public ulong AssetPanelID;
        public bool Visible;
    }

    [StructLayout(LayoutKind.Sequential)]
    public class UIPanelInstance
    {
        public UIPanel AssetPanel;
        public bool Visible;
    }
    [StructLayout(LayoutKind.Sequential)]
    public struct UILayer
    {
        public bool Visible;
        public int Index;

    }
}
