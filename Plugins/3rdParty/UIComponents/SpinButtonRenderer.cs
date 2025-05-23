using System;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;
using System.Drawing;

namespace UIComponents
{
	public enum SpinButton
	{
		Left,
		Right,
		Up,
		Down
	}

	// -----------------------------------------------------------

	public enum SpinState
	{
		Disabled,
		Hot,
		Pressed,
		Normal
	}

	// -----------------------------------------------------------

	public class SpinButtonRenderer
	{
		private static VisualStyleRenderer visualStyleRenderer = null;

		// -----------------------------------------------------------

		public static void Draw(Graphics graphics, Rectangle rect, SpinButton type, SpinState state)
		{
			if (!DrawThemed(graphics, rect, type, state))
				DrawUnthemed(graphics, rect, type, state);
		}

		public static bool DrawThemed(Graphics graphics, Rectangle rect, SpinButton type, SpinState state)
		{
			if (!VisualStyleRenderer.IsSupported)
				return false;

			VisualStyleElement vsBtn;

			// Up/Down buttons have 1px padding on 3 sides
			// Left/Right buttons have 1px padding on 2 sides
			// So we draw the button larger and clip out the padding
			var clipRect = rect;

			switch (type)
			{
			case SpinButton.Down:
				{
					// Padding left/right/bottom 
					//
					// #-----#
					// #     #
					// #     #
					// #######

					rect.X--;
					rect.Width += 2;

					rect.Height++;

					switch (state)
					{
					case SpinState.Disabled:
						vsBtn = VisualStyleElement.Spin.Down.Disabled;
						break;

					case SpinState.Hot:
						vsBtn = VisualStyleElement.Spin.Down.Disabled;
						break;

					case SpinState.Pressed:
						vsBtn = VisualStyleElement.Spin.Down.Pressed;
						break;

					case SpinState.Normal:
					default:
						vsBtn = VisualStyleElement.Spin.Down.Normal;
						break;
					}
				}
				break;

			case SpinButton.Up:
				{
					// Padding left/right/top 
					//
					// #######
					// #     #
					// #     #
					// #-----#

					rect.X--;
					rect.Width += 2;

					rect.Y--;
					rect.Height++;

					switch (state)
					{
					case SpinState.Disabled:
						vsBtn = VisualStyleElement.Spin.Up.Disabled;
						break;

					case SpinState.Hot:
						vsBtn = VisualStyleElement.Spin.Up.Disabled;
						break;

					case SpinState.Pressed:
						vsBtn = VisualStyleElement.Spin.Up.Pressed;
						break;

					case SpinState.Normal:
					default:
						vsBtn = VisualStyleElement.Spin.Up.Normal;
						break;
					}
				}
				break;

			case SpinButton.Left:
				{
					// Padding left/top 
					//
					// #######
					// #     |
					// #     |
					// #-----+

					rect.X--;
					rect.Width++;

					rect.Y--;
					rect.Height++;

					switch (state)
					{
					case SpinState.Disabled:
						vsBtn = VisualStyleElement.Spin.DownHorizontal.Disabled;
						break;

					case SpinState.Hot:
						vsBtn = VisualStyleElement.Spin.DownHorizontal.Disabled;
						break;

					case SpinState.Pressed:
						vsBtn = VisualStyleElement.Spin.DownHorizontal.Pressed;
						break;

					case SpinState.Normal:
					default:
						vsBtn = VisualStyleElement.Spin.DownHorizontal.Normal;
						break;
					}
				}
				break;

			case SpinButton.Right:
				{
					// Padding right/top
					//
					// #######
					// |     #
					// |     #
					// +-----#

					rect.Width++;

					rect.Y--;
					rect.Height++;

					switch (state)
					{
					case SpinState.Disabled:
						vsBtn = VisualStyleElement.Spin.UpHorizontal.Disabled;
						break;

					case SpinState.Hot:
						vsBtn = VisualStyleElement.Spin.UpHorizontal.Disabled;
						break;

					case SpinState.Pressed:
						vsBtn = VisualStyleElement.Spin.UpHorizontal.Pressed;
						break;

					case SpinState.Normal:
					default:
						vsBtn = VisualStyleElement.Spin.UpHorizontal.Normal;
						break;
					}
				}
				break;

			default:
				return false;
			}

			if (visualStyleRenderer == null)
				visualStyleRenderer = new VisualStyleRenderer(vsBtn.ClassName, vsBtn.Part, vsBtn.State);
			else
				visualStyleRenderer.SetParameters(vsBtn.ClassName, vsBtn.Part, vsBtn.State);

			visualStyleRenderer.DrawBackground(graphics, rect, clipRect);

			return true;
		}

		public static void DrawUnthemed(Graphics graphics, Rectangle rect, SpinButton type, SpinState state)
		{
			var scrollBtn = ScrollButton.Down;

			switch (type)
			{
			case SpinButton.Up:		scrollBtn = ScrollButton.Up;	break;
			case SpinButton.Left:	scrollBtn = ScrollButton.Left;	break;
			case SpinButton.Right:	scrollBtn = ScrollButton.Right;	break;
			}

			var scrollState = ButtonState.Normal;

			switch (state)
			{
			case SpinState.Disabled: scrollState = ButtonState.Inactive;	break;
			case SpinState.Pressed:	 scrollState = ButtonState.Pushed;		break;
			}

			ControlPaint.DrawScrollButton(graphics, rect, scrollBtn, scrollState);
		}
	}
}
