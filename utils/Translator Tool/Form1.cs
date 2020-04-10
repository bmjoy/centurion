using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;

namespace CenturionTranslatorTool
{
    public partial class Form1 : Form
    {
        string fileContent = string.Empty;
        string filePath = string.Empty;
        string fileName = "new.xml";
        string fileNameEmpty = "empty.xml";
        Model obj = null;

        public Form1()
        {
            InitializeComponent();
        }

        private void buttonOpen_Click(object sender, EventArgs e)
        {


            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.InitialDirectory = "c:\\";
                openFileDialog.Filter = "xml files (*.xml)|*.xml";
                openFileDialog.FilterIndex = 2;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {

                    var fileStream = openFileDialog.OpenFile();

                    fileContent = readXml(fileStream);

                    obj = Deserialize(fileContent);
                    modelToTable();

                    textBoxFileName.Text = fileName = openFileDialog.SafeFileName;
                    textBoxFilePath.Text = textBoxSavePath.Text = filePath = Path.GetDirectoryName(openFileDialog.FileName);
                }
            }

        }

        private void buttonRead_Click(object sender, EventArgs e)
        {
            fileName = textBoxFileName.Text;
            obj = tableToModel();
            saveCreateFile();

        }

        private static string readXml(Stream fileStream)
        {
            using (StreamReader reader = new StreamReader(fileStream))
            {
                return reader.ReadToEnd();
            }
        }

        private static Model Deserialize(string fileContent)
        {
            XmlSerializer ser = new XmlSerializer(typeof(Model));

            using (StringReader sr = new StringReader(fileContent))
            {
                return (Model)ser.Deserialize(sr);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            using (FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog())
            {

                if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
                {
                    filePath = textBoxFilePath.Text = textBoxSavePath.Text = folderBrowserDialog.SelectedPath;
                    textBoxFileName.Text = fileName;
                    fileContent = new StreamReader(File.OpenRead(fileNameEmpty)).ReadToEnd();
                    obj = Deserialize(fileContent);
                    modelToTable();
                    saveCreateFile();
                }
            }


        }

        private void modelToTable()
        {
            textLanguageName.Text = obj.Attribute1;
            dataGridView1.DataSource = null;
            dataGridView1.Rows.Clear();
            dataGridView1.ColumnCount = 2;
            dataGridView1.Columns[0].Name = "stringName";
            dataGridView1.Columns[0].ReadOnly = true;
            dataGridView1.Columns[1].Name = "result";

            dataGridView1.Columns[0].AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
            dataGridView1.Columns[1].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;

            foreach (MyItem i in obj.Items)
            {
                dataGridView1.Rows.Add(new string[] { i.StringName, i.Result });
            }
        }

        private Model tableToModel()
        {
            Model m = new Model();
            List<MyItem> items = new List<MyItem>();
            m.Attribute1 = textLanguageName.Text;
            foreach (DataGridViewRow row in dataGridView1.Rows)
            {
                MyItem item = new MyItem();
                item.StringName = row.Cells[0].Value != null ? row.Cells[0].Value.ToString() : "";
                item.Result = row.Cells[1].Value!=null ? row.Cells[1].Value.ToString() : "";
                items.Add(item);
            }
            m.Items = items;
            return m;
        }

        private void saveCreateFile()
        {
            var ser = new XmlSerializer(typeof(Model));
            XmlSerializerNamespaces ns = new XmlSerializerNamespaces();
            ns.Add("", "");
            var settings = new XmlWriterSettings();
            settings.OmitXmlDeclaration = true;
            settings.Indent = true;



            using (StreamWriter stream = new StreamWriter(filePath + @"\\" + fileName))
            {
                using (var writer = XmlWriter.Create(stream, settings))
                    ser.Serialize(writer, obj, ns);
            }
        }
    }
}
