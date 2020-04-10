using System.Collections.Generic;
using System.Xml.Serialization;

namespace CenturionTranslatorTool
{
    [XmlRoot("translationTable")]
    public class TranslationTable
    {
        [XmlAttribute("language")]
        public string Language { get; set; }

        [XmlElement("entry")]
        public List<Entry> Entries { get; set; }

    }

    public class Entry
    {
        [XmlAttribute("stringName")]
        public string StringName { get; set; }
        [XmlAttribute("result")]
        public string Result { get; set; }
    }
}
