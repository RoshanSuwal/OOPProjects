#pragma once

#include"Server.h"
#include"string.h"
#include<fstream>
#include <msclr\marshal_cppstd.h>

namespace FileSharingApp {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:

		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  tabControl1;
	protected:
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  ipAddressTextBox;

	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  shareBtnServer;
	private: System::Windows::Forms::Button^  shareBtnClient;


	private: System::Windows::Forms::ListBox^  listBox1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::ComponentModel::BackgroundWorker^  ServerBackgroundWorker;
	private: System::ComponentModel::BackgroundWorker^  ClientBackgroundWorker;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


		//Developer variables
	public:
		System::String^ ipAddress;
		System::String^name;
		System::String ^ path;

		System::String ^ sendingFileName;
		double sendingfileTotalSize = 0;
		double sentFilesize = 0;
		bool isFileSending = false;
		bool sendFile = false;

		bool uiupdateStarted = false;


	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::ListBox^  listBox2;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;

	public: System::Windows::Forms::ProgressBar^  progressBar1;
	private:
	public:





#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->ipAddressTextBox = (gcnew System::Windows::Forms::TextBox());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->listBox2 = (gcnew System::Windows::Forms::ListBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->shareBtnServer = (gcnew System::Windows::Forms::Button());
			this->shareBtnClient = (gcnew System::Windows::Forms::Button());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->ServerBackgroundWorker = (gcnew System::ComponentModel::BackgroundWorker());
			this->ClientBackgroundWorker = (gcnew System::ComponentModel::BackgroundWorker());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->SuspendLayout();
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Location = System::Drawing::Point(-4, 4);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(448, 405);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->button2);
			this->tabPage1->Controls->Add(this->button1);
			this->tabPage1->Controls->Add(this->label1);
			this->tabPage1->Controls->Add(this->ipAddressTextBox);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(440, 379);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"CONNECT";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(240, 237);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(124, 39);
			this->button2->TabIndex = 3;
			this->button2->Text = L"Start Client";
			this->button2->UseVisualStyleBackColor = true;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(67, 237);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(124, 39);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Start Server";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::SystemColors::ControlText;
			this->label1->Location = System::Drawing::Point(64, 88);
			this->label1->Name = L"label1";
			this->label1->Padding = System::Windows::Forms::Padding(2);
			this->label1->Size = System::Drawing::Size(86, 17);
			this->label1->TabIndex = 1;
			this->label1->Text = L"IP ADDRESS";
			// 
			// ipAddressTextBox
			// 
			this->ipAddressTextBox->Location = System::Drawing::Point(156, 88);
			this->ipAddressTextBox->Name = L"ipAddressTextBox";
			this->ipAddressTextBox->Size = System::Drawing::Size(208, 20);
			this->ipAddressTextBox->TabIndex = 0;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->label6);
			this->tabPage2->Controls->Add(this->label5);
			this->tabPage2->Controls->Add(this->label4);
			this->tabPage2->Controls->Add(this->label3);
			this->tabPage2->Controls->Add(this->listBox2);
			this->tabPage2->Controls->Add(this->label2);
			this->tabPage2->Controls->Add(this->progressBar1);
			this->tabPage2->Controls->Add(this->shareBtnServer);
			this->tabPage2->Controls->Add(this->shareBtnClient);
			this->tabPage2->Controls->Add(this->listBox1);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(440, 379);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"SHARE ZONE";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(26, 302);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(69, 13);
			this->label4->TabIndex = 7;
			this->label4->Text = L"received size";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(26, 268);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(52, 13);
			this->label3->TabIndex = 6;
			this->label3->Text = L"File name";
			// 
			// listBox2
			// 
			this->listBox2->FormattingEnabled = true;
			this->listBox2->Location = System::Drawing::Point(247, 29);
			this->listBox2->Name = L"listBox2";
			this->listBox2->Size = System::Drawing::Size(186, 212);
			this->listBox2->TabIndex = 5;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(26, 285);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(49, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"File Size:";
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(90, 247);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(288, 15);
			this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBar1->TabIndex = 3;
			// 
			// shareBtnServer
			// 
			this->shareBtnServer->Location = System::Drawing::Point(90, 329);
			this->shareBtnServer->Name = L"shareBtnServer";
			this->shareBtnServer->Size = System::Drawing::Size(213, 44);
			this->shareBtnServer->TabIndex = 2;
			this->shareBtnServer->Text = L"Share File";
			this->shareBtnServer->UseVisualStyleBackColor = true;
			this->shareBtnServer->Visible = false;
			this->shareBtnServer->Click += gcnew System::EventHandler(this, &MyForm::shareBtnServer_Click);
			// 
			// shareBtnClient
			// 
			this->shareBtnClient->Location = System::Drawing::Point(90, 329);
			this->shareBtnClient->Name = L"shareBtnClient";
			this->shareBtnClient->Size = System::Drawing::Size(213, 44);
			this->shareBtnClient->TabIndex = 1;
			this->shareBtnClient->Text = L"Share File";
			this->shareBtnClient->UseVisualStyleBackColor = true;
			this->shareBtnClient->Visible = false;
			this->shareBtnClient->Click += gcnew System::EventHandler(this, &MyForm::shareBtnClient_Click);
			// 
			// listBox1
			// 
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Location = System::Drawing::Point(29, 29);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(183, 212);
			this->listBox1->TabIndex = 0;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// ServerBackgroundWorker
			// 
			this->ServerBackgroundWorker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MyForm::ServerBackgroundWorker_DoWork);
			// 
			// timer1
			// 
			this->timer1->Interval = 2;
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(29, 7);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(48, 13);
			this->label5->TabIndex = 8;
			this->label5->Text = L"Sent File";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(247, 7);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(65, 13);
			this->label6->TabIndex = 9;
			this->label6->Text = L"Pending File";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(445, 408);
			this->Controls->Add(this->tabControl1);
			this->Name = L"MyForm";
			this->Text = L"FILE SHARING APP";
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			this->ResumeLayout(false);

		}

	private: System::Void shareBtnServer_Click(System::Object^  sender, System::EventArgs^  e) {
		openFileDialog1->ShowDialog();
		name = openFileDialog1->SafeFileName;
		path = openFileDialog1->FileName;
		if (name->Length != 0) {
			//Console::WriteLine(path);
			sendFile = true;
			listBox2->Items->Add(path);
		}
		if (!uiupdateStarted) { this->timer1->Start(); }

	}
	private: System::Void shareBtnClient_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		ipAddress = ipAddressTextBox->Text;
		if (ipAddress->Length != 0) {
			ServerBackgroundWorker->RunWorkerAsync();

			shareBtnServer->Visible = true;
			tabControl1->SelectedIndex = 1;

		}

		


	}
	private: System::Void ServerBackgroundWorker_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
		Server server = Server(9998);
		server.ListenForNewConnection();
		while (true) {
			if (server.newFileRequest) {
				std::string name = server.s_sendingFileName;
				sendingFileName = gcnew String(name.c_str());
				sendingfileTotalSize = server.s_totalsize;
				server.newFileRequest = false;
				isFileSending = true;
				std::cout << sendingfileTotalSize << std::endl;
				Console::Write(sendingfileTotalSize);
			}

			if (server.fileNotsentYet) {
				Console::WriteLine(sentFilesize);
				
				sentFilesize = server.s_sentfileSize;

				if (sentFilesize == sendingfileTotalSize) {
					Console::WriteLine(sendingfileTotalSize);
				}
					
			}


			if (sendFile) {
				Console::WriteLine(path);

				/*std::string fileName, filePath;

				StreamWriter^outFile = gcnew StreamWriter("D:\\visual studio project\\form2\\debug\\Test.txt");
				outFile->Write(name);

				outFile->Close();

				std::fstream file("D:\\visual studio project\\form2\\debug\\Test.txt");
				while (file)
				{

					if (file.eof() == 1) {
						//mSendMessage = false;
						file.close();
						break;
					}
					else {
						std::getline(file, fileName);

						std::cout << fileName << std::endl;
						file.close();
						//server.convertToCompatible(filePath);
						//server.addToSend(fileName, filePath);
						break;
					}
				}

				StreamWriter^utFile = gcnew StreamWriter("D:\\visual studio project\\form2\\debug\\Test.txt");
				utFile->Write(path);
				utFile->Close();

				std::fstream fil("D:\\visual studio project\\form2\\debug\\Test.txt");
				while (fil)
				{

					if (fil.eof() == 1) {
						//mSendMessage = false;
						sendFile = false;
						fil.close();
						break;
					}
					else {

						std::getline(fil, filePath);
						std::cout << fileName << std::endl;
						std::cout << filePath << std::endl;
						sendFile = false;
						fil.close();
						//server.convertToCompatible(filePath);
						server.addToSend(fileName, filePath);
						break;
					}
				}
				std::string msg = msclr::interop::marshal_as<std::string>(s);
				*/

				std::string fileName, filePath;
				String ^fname = name;
				fileName = msclr::interop::marshal_as<std::string>(fname);
				fname = path;
				filePath = msclr::interop::marshal_as<std::string>(fname);

				std::cout << fileName << "     " << filePath << std::endl;
				server.addToSend(fileName, filePath);

				sendFile = false;

			}
		}

	}
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
		if (isFileSending) {
			label3->Text = "File name : "+sendingFileName;
			label2->Text = "File Size : "+(sendingfileTotalSize / 1024).ToString() + " KB";
			label4->Text = "Size Received : "+(sentFilesize/1024).ToString()+" KB";
			progressBar1->Value = sentFilesize * 100 / sendingfileTotalSize;
			progressBar1->Text= sentFilesize.ToString();
			if (progressBar1->Value == 100) {
				 listBox2->Items->RemoveAt(0); 
				listBox1->Items->Add(sendingFileName);	
				label4->Text = "File sending Completeled";
				//progressBar1->Value = 0;
				sentFilesize = 0;
				isFileSending = false;
			}
		}
	}

};
}
