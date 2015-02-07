# coding:utf-8
import os
from reportlab.pdfgen import canvas
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.lib.pagesizes import landscape, portrait
from reportlab.lib.pagesizes import A3, B3
from reportlab.lib.pagesizes import A4, B4
from reportlab.lib.pagesizes import A5, B5

from reportlab.pdfbase.cidfonts import UnicodeCIDFont
# フォントの登録
pdfmetrics.registerFont(TTFont("Bitstream Vera Sans", "Vera.ttf"))

pdfmetrics.registerFont(TTFont("ＭＳ 明朝", "MS Mincho.ttf"))
pdfmetrics.registerFont(UnicodeCIDFont("HeiseiKakuGo-W5"))
pdfmetrics.registerFont(UnicodeCIDFont("HeiseiMin-W3"))

py = os.path.abspath(__file__)
# from reportlab.pdfbase.cidfonts import UnicodeCIDFont
# fontname = "HeiseiKakuGo-W5"
# fontname = "HeiseiMin-W3"
# pdfmetrics.registerFont(UnicodeCIDFont(fontname))

class point:
	def __init__(self, x, y):
		self.x, self.y = x, y
	def copy(self):
		return point(self.x, self.y)
class size:
	def __init__(self, w, h):
		self.w, self.h = w, h

class cell:
	def __init__(self):
		self.initialize()
	
	def initialize(self):
		fontname = self.fontname = "Helvetica"
		fontname = self.fontname = "HeiseiKakuGo-W5"
		fontname = self.fontname = "Bitstream Vera Sans"
		fontname = self.fontname = "ＭＳ 明朝"
		fontname = self.fontname = "HeiseiMin-W3"
		fontsize = self.fontsize = 14
		self.charspace = 0.0
		self.wordspace = 0.0
		self.practice = False
		return self

	def create(self, filename, pagesize = portrait(A4), margin = point(40, 40)):

		fontname = self.fontname
		fontsize = self.fontsize
		self.canvas = canvas.Canvas(filename
			, pagesize=pagesize
			# , pageCompression=0
			, pageCompression=1
			, pdfVersion=(1, 4)
			)
		self.canvas.setFont(fontname, fontsize)

		self.margin = margin
		self.pagesize = size(
			self.canvas._pagesize[0], 
			self.canvas._pagesize[1])
		self.where = point(0, 0)
		self.lineheight = 20#★★

		return self

	def setfont(self, name = None, size = None):
		if not name is None:
			self.fontname = name
		if not size is None:
			self.fontsize = size
		self.canvas.setFont(self.fontname, self.fontsize)
		return self

	def setcharspace(self, charspace):
		self.charspace = charspace
		return self
	def setwordspace(self, wordspace):
		self.wordspace = wordspace
		return self

	def moveto(self, x, y):
		self.where = point(x, y).copy()
		return self
	def move(self, x, y):
		self.where.x += x
		self.where.y += y
		return self
	def cr(self, lineheight = None):
		if not lineheight is None:
			self.lineheight = lineheight
		return self.move(0, self.lineheight)
	def home(self):
		self.where.x = 0
		return self
	def yhome(self):
		self.where.y = 0
		return self
	def end(self):
		self.where.x = self.pagesize.w - self.margin.x
		return self
	def yend(self):
		self.where.y = self.pagesize.h - self.margin.y
		return self
	def reset(self):
		return self.home().yhome()
	def real(self, where):
		margin = self.margin
		pagesize = self.pagesize
		return point(margin.x + where.x, pagesize.h - (margin.y + where.y))

	def draw(self, text):
		if self.practice: return self#★★★
		canvas = self.canvas
		real = self.real(self.where)
		t = canvas.beginText(real.x, real.y)
		t.textLine(text)
		t.setCharSpace(self.charspace)
		t.setWordSpace(self.wordspace)
		canvas.drawText(t)
		return self
	def line(self, nw, se, linewidth = None, dash = None):
		canvas = self.canvas
		a = self.real(nw)
		b = self.real(se)
		if linewidth > 0:
			canvas.setLineWidth(linewidth)
		if dash: 
			canvas.setDash(dash)
		canvas.line(a.x, a.y, b.x, b.y)
		return self

	def showPage(self):
		if self.practice: return self#★★★
		self.canvas.showPage()
		return self
	def save(self):
		if self.practice: return self#★★★
		self.canvas.save()
		return self
