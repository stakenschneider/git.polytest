import UIKit
import Foundation

let myLbl = UILabel(frame: CGRectMake(0,0,300,500))
myLbl.text = "dfnjv"

func swap(inout a: Int , inout b: Int){
    let temp = a
    a = b
    b = temp
}

var a = 10; var b = 20
swap(&a, b: &b)

func blaBla(aa: Int , bb: Int) ->Int{return aa+bb}
var blaVar: (Int,Int)->Int = blaBla
blaVar(1,2)
func blaFunc (aFunc: (Int,Int)->Int) ->Int
{
    return aFunc(11,22)
}
blaFunc(blaVar)

func seria (nums:Double...)->Double
{
    var total:Double = 0
    for num in nums{total+=num}
    return total
}
seria(11,22,33,44,0.11,102)

let русс = ["ма" ,"ывс" ,"ми" , "его"]
русс.sort(<)


for i in 1...5 {for var value = 0.0; value < M_PI * 4; value += 0.1 {var y = cos(value)}}

class GradientView : UIView {
    
    override func drawRect(rect: CGRect) {
        
        let context : CGContextRef = UIGraphicsGetCurrentContext()!
        
        let dotBlue = UIColor(red: 39.0/255.0, green: 119.0/255.0, blue: 174.0/255.0, alpha:1.0 ).CGColor
        let lineBlue = UIColor(red: 135.0/255.0, green: 80.0/255.0, blue: 145.0/255.0, alpha:0.2 ).CGColor
        let startBlue = UIColor(red: 27.0/255.0, green: 94.0/255.0, blue: 125.0/255.0, alpha:1.0 ).CGColor
        let endBlue = UIColor(red: 144.0/255.0, green: 60.0/255.0, blue: 110.0/255.0, alpha:1.0 ).CGColor
        
        let locations: [CGFloat] = [0.0, 0.75]
        
        let colors = [startBlue, endBlue]
        
        let colorSpace = CGColorSpaceCreateDeviceRGB()
        
        let gradient : CGGradientRef = CGGradientCreateWithColors(colorSpace, colors, locations)!
        
        let startPoint : CGPoint = CGPointMake(0.0, 0.0)
        let endPoint : CGPoint = CGPointMake(rect.size.width,rect.size.height)
        
        CGContextDrawLinearGradient(context, gradient,startPoint, endPoint , CGGradientDrawingOptions())
        
        let diam : CGFloat = 6.0
        let radius : CGFloat = diam / 2.0
        let step : CGFloat = 60.0
        let startX : CGFloat = (rect.size.width % step) * 0.5
        let startY : CGFloat = (rect.size.height % step) * 0.5
        
        CGContextSetStrokeColorWithColor(context, lineBlue)
        CGContextSetLineWidth(context, 2.0)
        
        for var i = startX; i < rect.size.width; i += 2.0 * step {
            CGContextMoveToPoint(context, i, 0.0)
            CGContextAddLineToPoint(context, i, rect.size.height)
            CGContextSetShadowWithColor(context, CGSizeMake(0, 0), 4.0, lineBlue)
        }
        
        for var j = startX; j < rect.size.width; j += 2.0 * step {
            CGContextMoveToPoint(context, 0.0, j)
            CGContextAddLineToPoint(context, rect.size.width, j)
            CGContextSetShadowWithColor(context, CGSizeMake(0, 0), 4.0, lineBlue)
        }
        
        CGContextStrokePath(context)
        
        for var i = startX; i < rect.size.width; i += step {
            
            for var j = startY; j < rect.size.height; j += step {
                
                let point = CGPointMake(rect.origin.x + i, rect.origin.y + j)
                
                CGContextSetShadowWithColor(context, CGSizeMake(0, 0), 4.0, dotBlue)
                
                CGContextSetFillColorWithColor(context, dotBlue)
                
                let circle = CGRectMake(point.x - radius, point.y-radius, diam, diam)
                
                CGContextAddEllipseInRect(context, circle)
            }
            
        }
        
        CGContextFillPath(context)
    }
    
}

var baseView = GradientView (frame:CGRectMake(0.0, 0.4, 600.0, 600.0))
