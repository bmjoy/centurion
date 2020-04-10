using System.Collections.Generic;
using System.Xml.Serialization;

namespace CenturionTranslatorTool
{
    [XmlRoot("translationTable")]
    public class Model
    {
        [XmlAttribute("language")]
        public string Attribute1 { get; set; }

        [XmlElement("entry")]
        public List<MyItem> Items { get; set; }

    }

    public class MyItem
    {
        [XmlAttribute("stringName")]
        public string StringName { get; set; }
        [XmlAttribute("result")]
        public string Result { get; set; }
    }
}
