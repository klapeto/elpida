using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Elpida.Mobile.Views
{
	public partial class TopologyNodeView : ContentView
	{
		public TopologyNodeView()
		{
			InitializeComponent();
		}
		
		public Color NodeColor { get; set; } = Color.FromRgb( 0, 255, 0);
	}
}