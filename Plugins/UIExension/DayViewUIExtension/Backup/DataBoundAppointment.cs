using System;
using System.Collections.Generic;
using System.Text;
using System.Data;

namespace Calendar
{
    public class DataBoundAppointment : Appointment
    {
        public DataBoundAppointment(DataRow row)
        {
            if (row == null)
                throw new ArgumentNullException("row");

            this.row = row;
        }

        DataRow row;

        public DataRow Row
        {
            get
            {
                return row;
            }
        }

        protected override void OnStartDateChanged()
        {
            base.OnStartDateChanged();

            row["StartDate"] = this.StartDate;
        }

        protected override void OnEndDateChanged()
        {
            base.OnEndDateChanged();

            row["EndDate"] = this.EndDate;
        }

        protected override void OnLockedChanged()
        {
            base.OnLockedChanged();

            row["Locked"] = this.Locked;
        }

        protected override void OnTitleChanged()
        {
            base.OnTitleChanged();

            row["Title"] = this.Title;
        }

        protected override void OnColorChanged()
        {
            base.OnColorChanged();

            row["Color"] = this.Color.ToArgb();
        }

        protected override void OnBorderColorChanged()
        {
            base.OnBorderColorChanged();

            row["BorderColor"] = this.BorderColor.ToArgb();
        }

        protected override void OnTextColorChanged()
        {
            base.OnTextColorChanged();

            row["TextColor"] = this.TextColor.ToArgb();
        }
    }
}
