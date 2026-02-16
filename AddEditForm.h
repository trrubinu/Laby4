#pragma once

#include "CargoCompany.h"
#include <msclr/marshal_cppstd.h>
#include <vcclr.h>
#include <string>
#include <limits>

namespace CargoCompanyWF {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    System::String^ FormUtf8(const char* utf8);

    public ref class AddEditForm : public Form
    {
    public:
        AddEditForm(CargoCompany* company, int mode);
        AddEditForm(CargoCompany* company, int mode, int index);

    private:
        CargoCompany* company;
        int mode;
        int editIndex;

        Label^ lbl1;
        Label^ lbl2;
        Label^ lbl3;
        TextBox^ txt1;
        TextBox^ txt2;
        TextBox^ txt3;
        ComboBox^ comboDiscountType;
        TextBox^ txtDiscountValue;
        ComboBox^ comboTariffs;
        Button^ btnOK;
        Button^ btnCancel;

        void InitializeComponent();
        void ConfigureMode();
        void LoadDataForEdit();
        bool ValidateInputs();

        void OnDiscountTypeChanged(Object^ sender, EventArgs^ e);
        void OnOK(Object^ sender, EventArgs^ e);
    };
}
