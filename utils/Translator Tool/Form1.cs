using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;

namespace CenturionTranslatorTool
{
    public partial class Form1 : Form
    {
        private readonly List<TranslationTable> listOfTranslationTables = new List<TranslationTable>();
        private string[] xmlFiles;
        private readonly List<string> languages = new List<string>();

        public Form1()
        {
            InitializeComponent();
            LoadTables();
        }

        public void LoadTables()
        {
            xmlFiles = GetFileNames("tables/", "*.xml");

            foreach (string xmlFile in xmlFiles)
            {
                string fileContent = File.ReadAllText("tables/" + xmlFile);
                string language = xmlFile.Split('_')[1].Split('.')[0];
                languages.Add(language);
                TranslationTable table = Deserialize(fileContent);
                listOfTranslationTables.Add(table);
            }

            if (listOfTranslationTables.Count > 0)
            {
                CreateTable();
            }
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            List<TranslationTable> tables = GetXmlsFromViewTable();

            for (int K = 0; K < languages.Count; K++)
            {
                TranslationTable tab = tables[K];
                SerializeXml(languages[K], ref tab);
            }
        }

        private void buttonNewLanguage_Click(object sender, EventArgs e)
        {
            string input = Microsoft.VisualBasic.Interaction.InputBox("Please write the name of the language", "Add a new language", "language", 100, 100);

            if (input != "")
            {
                dataGridView1.Columns.Add(input, input);
                languages.Add(input);
            }
        }

        private void buttonNewEntry_Click(object sender, EventArgs e)
        {
            string[] newRow = new string[languages.Count + 1];
            dataGridView1.Rows.Add(newRow);
        }

        private void buttonSort_Click(object sender, EventArgs e)
        {
            dataGridView1.Sort(dataGridView1.Columns["stringName"], ListSortDirection.Ascending);
        }

        private static string[] GetFileNames(string path, string filter)
        {
            string[] files = Directory.GetFiles(path, filter);
            for (int i = 0; i < files.Length; i++)
                files[i] = Path.GetFileName(files[i]);
            return files;
        }

        private static TranslationTable Deserialize(string fileContent)
        {
            XmlSerializer ser = new XmlSerializer(typeof(TranslationTable));

            using (StringReader sr = new StringReader(fileContent))
            {
                return (TranslationTable)ser.Deserialize(sr);
            }
        }

        private void CreateTable()
        {
            dataGridView1.DataSource = null;
            dataGridView1.Rows.Clear();
            SetFontAndColors();
            dataGridView1.ColumnCount = listOfTranslationTables.Count + 1;
            dataGridView1.Columns[0].Name = "stringName";
            dataGridView1.Columns[0].AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;

            int K = 0;
            foreach (TranslationTable t in listOfTranslationTables)
            {
                dataGridView1.Columns[K + 1].Name = languages[K];
                dataGridView1.Columns[K + 1].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
                K += 1;
            }

            for (int row = 0; row < listOfTranslationTables[0].Entries.Count; row++)
            {
                string[] entries = new string[listOfTranslationTables.Count + 1];
                entries[0] = listOfTranslationTables[0].Entries[row].StringName;

                K = 0;
                foreach (TranslationTable t in listOfTranslationTables)
                {
                    entries[K + 1] = t.Entries[row].Result == "--" ? "" : t.Entries[row].Result;
                    K += 1;
                }

                dataGridView1.Rows.Add(entries);
            }
        }

        private List<TranslationTable> GetXmlsFromViewTable()
        {
            List<TranslationTable> tables = new List<TranslationTable>();
            int iCol = 0;
            foreach (DataGridViewColumn col in dataGridView1.Columns)
            {
                TranslationTable m = new TranslationTable();
                List<Entry> items = new List<Entry>();
                int iRow = 0;
                if (iCol > 0)
                {
                    foreach (DataGridViewRow row in dataGridView1.Rows)
                    {
                        string string_name = row.Cells[0].Value != null ? row.Cells[0].Value.ToString() : "";
                        if (String.IsNullOrEmpty(string_name)) continue;
                        string result = row.Cells[iCol].Value != null ? row.Cells[iCol].Value.ToString() : "";

                        Entry item = new Entry
                        {
                            StringName = string_name,
                            Result = String.IsNullOrEmpty(result) ? "--" : result
                        };

                        if (String.IsNullOrEmpty(item.StringName))
                        {
                            continue;
                        }
                        items.Add(item);
                        iRow++;
                    }
                    m.Language = languages[iCol - 1];
                    m.Entries = items;
                    tables.Add(m);
                }
                iCol++;
            }
            return tables;
        }

        private void SerializeXml(string lan, ref TranslationTable tab)
        {
            var ser = new XmlSerializer(typeof(TranslationTable));
            XmlSerializerNamespaces ns = new XmlSerializerNamespaces();
            ns.Add("", "");

            var settings = new XmlWriterSettings
            {
                OmitXmlDeclaration = true,
                Indent = true
            };

            using (StreamWriter stream = new StreamWriter("tables/translationTable_" + lan + ".xml"))
            {
                using (var writer = XmlWriter.Create(stream, settings))
                {
                    ser.Serialize(writer, tab, ns);
                }
            }
        }

        private void Form1_Resize(object sender, EventArgs e)
        {
            dataGridView1.Width = this.Width - 40;
            dataGridView1.Height = this.Height - 100;
            button1.Location = new Point(button1.Location.X, this.Height - 75);
            buttonNewEntry.Location = new Point(buttonNewEntry.Location.X, this.Height - 75);
            buttonSave.Location = new Point(buttonSave.Location.X, this.Height - 75);
            buttonNew.Location = new Point(buttonNew.Location.X, this.Height - 75);
        }

        private void SetFontAndColors()
        {
            dataGridView1.BackgroundColor = Color.LightGray;
            dataGridView1.DefaultCellStyle.SelectionBackColor = Color.White;
            dataGridView1.DefaultCellStyle.SelectionForeColor = Color.Black;
            dataGridView1.RowHeadersDefaultCellStyle.SelectionBackColor = Color.Empty;
            dataGridView1.RowsDefaultCellStyle.BackColor = Color.LightGray;
            dataGridView1.AlternatingRowsDefaultCellStyle.BackColor = Color.DarkGray;
            dataGridView1.ColumnHeadersDefaultCellStyle.ForeColor = Color.White;
            dataGridView1.ColumnHeadersDefaultCellStyle.BackColor = Color.Black;
            dataGridView1.RowHeadersDefaultCellStyle.BackColor = Color.Black;
        }
    }
}