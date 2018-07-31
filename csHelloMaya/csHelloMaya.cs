using System;
using Autodesk.Maya.OpenMaya;

[assembly:MPxCommandClass(typeof(MayaCSPlugin.csHelloMaya), "csHelloMaya")]
[assembly:ExtensionPlugin(typeof(MayaCSPlugin.csHelloMayaPlugin), "kingmax_res@163.com | 184327932@qq.com | iJasonLee@WeChat")]

namespace MayaCSPlugin
{

    public class csHelloMayaPlugin : IExtensionPlugin
    {
        public string GetMayaDotNetSdkBuildVersion()
        {
            return "2018.07.25.01";
        }

        public bool InitializePlugin()
        {
            return true;
        }

        public bool UninitializePlugin()
        {
            return true;
        }
    }

    public class csHelloMaya : MPxCommand, IMPxCommand
    {
        public override void doIt(MArgList args)
        {
            //base.doIt(args);
            MGlobal.displayInfo("Hello, Maya. This is from Maya CSharp Plugin! 你好！");
        }
    }
}
