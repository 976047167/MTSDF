// Learn TypeScript:
//  - https://docs.cocos.com/creator/manual/en/scripting/typescript.html
// Learn Attribute:
//  - https://docs.cocos.com/creator/manual/en/scripting/reference/attributes.html
// Learn life-cycle callbacks:
//  - https://docs.cocos.com/creator/manual/en/scripting/life-cycle-callbacks.html

const {ccclass, property} = cc._decorator;

@ccclass
export default class SDFLabel extends cc.Label {
	@property
	_outlineColor:cc.Color = cc.Color.BLACK
	@property
	get outlineColor() {
		return this._outlineColor
	}
	set outlineColor(value:cc.Color) {
		this._outlineColor = value;
		this._forceUpdateRenderData();
	}

	@property
	_outlineSize:number = 0.2
	@property
	get outlineSize() {
		return this._outlineSize
	}
	set outlineSize(value:number) {
		this._outlineSize= value;
		this._forceUpdateRenderData();
	}
	protected _updateMaterialWebgl() {
		let material = this.getMaterial(0);
		if (this.font instanceof cc.BitmapFont) {
			material && material.setProperty('outlineColor', this._outlineColor);
			material && material.setProperty('outlineSize', this._outlineSize);
			const a =1/this.font.spriteFrame._texture.width*3;
			const b =1/this.font.spriteFrame._texture.height*3;
			material && material.setProperty('unitWidth', a);
			material && material.setProperty('unitHeight', b);
		} 

		super._updateMaterialWebgl();

	}
}
